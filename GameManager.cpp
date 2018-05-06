#include "GameManager.h"

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
}


GameManager::~GameManager()
{
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
	if (gameTime >= 0 && ringOfFire >= 20)
	{
		Debug.Log(ringOfFire);  
		ringOfFire -= 0.5f * Time.getDeltaTime() * gameTime;
		devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &ringOfFire, 0, 0);
	}

	//DirectX::XMVECTOR middle = DirectX::XMVectorSet(99, 0, 99, 0);
	//DirectX::XMVECTOR eyy = DirectX::XMVectorSet(200, 0, 200, 0);
	//if (DirectX::XMVectorGetX(DirectX::XMVector3LinePointDistance(middle, middle, eyy)) <= ringOfFire)
	//	Debug.Log(ringOfFire);//take dmg
}
