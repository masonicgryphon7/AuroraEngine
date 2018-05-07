#include "GameManager.h"




std::vector<std::vector<Unit*>> GameManager::unitLists;
GAME_STATE GameManager::gameState;


GameManager::GameManager()
{

}

GameManager::GameManager(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	ringOfFire = 150;
	gameTime = 0;
	dev = gDevice;
	devCon = gDeviceContext;
	createBuffer(gDevice, gDeviceContext);

	unitLists = std::vector<std::vector<Unit*>>(3, std::vector<Unit*>());
	gameState = GAME_STATE::LARGE_CIRCEL_STATE;
}


GameManager::~GameManager()
{
}

void GameManager::dmgRing()
{
	for (int i = 0; i < unitLists[2].size(); i++)
	{
		if (unit->getDistanceBetweenUnits(unitLists[2][i]->gameObject->transform.getPosition(), middlePoint) > ringOfFire)
		{
			Debug.Log("eeeeeeeeeeey");
			unitLists[2][i]->takeDamage(100);
		}
	}
	for (int i = 0; i < unitLists[1].size(); i++)
	{
		if (unit->getDistanceBetweenUnits(unitLists[1][i]->gameObject->transform.getPosition(), middlePoint) > ringOfFire)
		{
			Debug.Log("eeeeeeeeeeey");
			unitLists[1][i]->takeDamage(100);
		}
	}
}

HRESULT GameManager::createBuffer(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	HRESULT hr = S_OK;
	Manager_Buffer Manager_BufferData;
	Manager_BufferData.fireRing = DirectX::XMVectorSet(ringOfFire, ringOfFire, ringOfFire, ringOfFire);

	CD3D11_BUFFER_DESC ManagerDesc(
		sizeof(Manager_Buffer),
		D3D11_BIND_CONSTANT_BUFFER);


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &Manager_BufferData;
	gDevice->CreateBuffer(&ManagerDesc, &InitData, &GameManagerBuffer);
	gDeviceContext->PSSetConstantBuffers(1, 1, &GameManagerBuffer);
	return hr;
}

void GameManager::update()
{
	gameTime += Time.getDeltaTime();
	switch (gameState)
	{
	case MAIN_MENU:
		break;
	case START_STATE:
		break;
	case LARGE_CIRCEL_STATE:
		if (gameTime >= 3)
		{
			//Debug.Log(ringOfFire);  
			ringOfFire -= 0.05f * Time.getDeltaTime() * gameTime;
			devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &ringOfFire, 0, 0);
			dmgRing();
		}
		if (ringOfFire < 100) {
			gameState = GAME_STATE::MEDIUM_CIRCEL_STATE;
		}
		break;
	case MEDIUM_CIRCEL_STATE:
		if (gameTime >= 60)
		{
			//Debug.Log(ringOfFire);  
			ringOfFire -= 0.05f * Time.getDeltaTime() * gameTime;
			devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &ringOfFire, 0, 0);
			dmgRing();
		}
		if (ringOfFire < 70) {
			gameState = GAME_STATE::SMALL_CIRCEL_STATE;
		}
		break;
	case SMALL_CIRCEL_STATE:
		if (gameTime >= 90 && ringOfFire >= 20)
		{
			//Debug.Log(ringOfFire);  
			ringOfFire -= 0.05f * Time.getDeltaTime() * gameTime;
			devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &ringOfFire, 0, 0);
		}
		dmgRing();
		break;
	case END_STATE:
		break;
	case GAME_OVER_MENU:
		break;
	default:
		break;
	}


	//DirectX::XMVECTOR middle = DirectX::XMVectorSet(99, 0, 99, 0);
	//DirectX::XMVECTOR eyy = DirectX::XMVectorSet(200, 0, 200, 0);
	//if (DirectX::XMVectorGetX(DirectX::XMVector3LinePointDistance(middle, middle, eyy)) <= ringOfFire)
	//	Debug.Log(ringOfFire);//take dmg
}
