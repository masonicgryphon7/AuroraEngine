#include "GameManager.h"




std::vector<std::vector<Unit*>> GameManager::unitLists;
std::vector<std::vector<Unit*>> GameManager::buildingLists;
GAME_STATE GameManager::gameState;
RING_STATE GameManager::ringState;
float GameManager::ringOfFire;

GameManager::GameManager()
{

}

GameManager::GameManager(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	ringOfFire = 220;
	gameTime = 0;
	dev = gDevice;
	devCon = gDeviceContext;
	createBuffer(gDevice, gDeviceContext);

	unitLists = std::vector<std::vector<Unit*>>(4, std::vector<Unit*>());
	buildingLists = std::vector<std::vector<Unit*>>(4, std::vector<Unit*>());

	gameState = GAME_STATE::MAIN_MENU;
}


GameManager::~GameManager()
{
}

void GameManager::dmgRing()
{
	for (int i = 0; i < unitLists.size(); i++)
	{

		for (int j = 0; j < unitLists[i].size(); j++)
		{
			if (unit->getDistanceBetweenUnits(unitLists[i][j]->gameObject->transform.getPosition(), middlePoint) > ringOfFire)
			{
				//Debug.Log("eeeeeeeeeeey");
				unitLists[i][j]->takeFireDamage(1 * Time.getDeltaTime());
			}
		}
	
	}
	for (int i = 0; i < buildingLists.size(); i++)
	{
		for (int j = 0; j < buildingLists[i].size(); j++)
		{
			if (unit->getDistanceBetweenUnits(buildingLists[i][j]->gameObject->transform.getPosition(), middlePoint) > ringOfFire)
			{
				buildingLists[i][j]->takeFireDamage(1 * Time.getDeltaTime());
			}
		}
	}
	//for (int i = 0; i < unitLists[1].size(); i++)
	//{
	//	if (unit->getDistanceBetweenUnits(unitLists[1][i]->gameObject->transform.getPosition(), middlePoint) > ringOfFire)
	//	{
	//		//Debug.Log("eeeeeeeeeeey");
	//		unitLists[1][i]->takeFireDamage(1 * Time.getDeltaTime());
	//		
	//	}
	//}
	
}

HRESULT GameManager::createBuffer(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	HRESULT hr = S_OK;
	Manager_Buffer Manager_BufferData;
	Manager_BufferData.fireRing = DirectX::XMVectorSet(ringOfFire, Time.getDeltaTime(), ringOfFire, ringOfFire);

	CD3D11_BUFFER_DESC ManagerDesc(
		sizeof(Manager_Buffer),
		D3D11_BIND_CONSTANT_BUFFER);


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &Manager_BufferData;
	HRESULT res= gDevice->CreateBuffer(&ManagerDesc, &InitData, &GameManagerBuffer);
	gDeviceContext->PSSetConstantBuffers(1, 1, &GameManagerBuffer);
	return hr;
}

void GameManager::winCondition()
{
	if (unitLists[1][0]->getHealthPoints() <= 0)
	{
		gameState = GAME_STATE::GAME_OVER_MENU;
	}
	if (unitLists[2][0]->getHealthPoints() <= 0)
	{
		gameState = GAME_STATE::GAME_OVER_MENU;
	}
}

void GameManager::update()
{
	gameTime += Time.getDeltaTime();
	uvPanning += lavaSpeed * Time.getDeltaTime();
	if (uvPanning > 1)
		uvPanning = 0;

	fireBufferData = DirectX::XMFLOAT4(ringOfFire, uvPanning, 0, 0);
	devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &fireBufferData, 0, 0);
	//winCondition();
	switch (gameState)
	{
	case MAIN_MENU:
		ringOfFire = 220;
		gameTime = 0;
		for (auto i : gamemanager.unitLists)
		{
			for (auto j : i)
			{
				if (j == nullptr) continue;

				GameObject* temp = j->gameObject;
				temp->Destroy();
			}
		}
		gamemanager.unitLists.shrink_to_fit();
		gamemanager.unitLists.clear();

		for (auto i : gamemanager.buildingLists)
		{
			for (auto j : i)
			{
				if (j == nullptr) continue;

				GameObject* temp = j->gameObject;
				temp->Destroy();
			}
		}
		gamemanager.buildingLists.shrink_to_fit();
		gamemanager.buildingLists.clear();
		break;
	case START_STATE:
		gameState = LARGE_CIRCLE_STATE;
		ringState = NO_MOVE;
		break;
	case LARGE_CIRCLE_STATE:
		if (gameTime >= 12)
		{
			//Debug.Log(ringOfFire); 
			ringState = RING_STATE::MOVE;
			ringOfFire -= 0.02f * Time.getDeltaTime() * gameTime;
		}
		if (ringOfFire < 150) {
			gameState = GAME_STATE::MEDIUM_CIRCLE_STATE;
			ringState = RING_STATE::NO_MOVE;
			lavaSpeed = 0.01;
		}
		dmgRing();
		break;
	case MEDIUM_CIRCLE_STATE:
		if (gameTime >= 18)
		{
			//Debug.Log(ringOfFire);  
			//devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &ringOfFire, 0, 0);
			ringState = RING_STATE::MOVE;
			ringOfFire -= 0.02f * Time.getDeltaTime() * gameTime;
			lavaSpeed = 0.05;
		}
		if (ringOfFire < 80) {
			gameState = GAME_STATE::SMALL_CIRCLE_STATE;
			ringState = RING_STATE::NO_MOVE;
			lavaSpeed = 0.01;
		}
		dmgRing();
		break;
	case SMALL_CIRCLE_STATE:
		if (gameTime >= 240 && ringOfFire > 28)
		{
			//Debug.Log(ringOfFire);  
			//devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &ringOfFire, 0, 0);
			ringState = RING_STATE::MOVE;
			ringOfFire -= 0.02f * Time.getDeltaTime() * gameTime;
			lavaSpeed = 0.05;
		}
		else
		{
			lavaSpeed = 0.01;
		}
		dmgRing();
		break;
	case END_STATE:
		break;
	case GAME_OVER_MENU:
		Debug.Log("END BOOIOII");
		break;
	default:
		break;
	}


	//DirectX::XMVECTOR middle = DirectX::XMVectorSet(99, 0, 99, 0);
	//DirectX::XMVECTOR eyy = DirectX::XMVectorSet(200, 0, 200, 0);
	//if (DirectX::XMVectorGetX(DirectX::XMVector3LinePointDistance(middle, middle, eyy)) <= ringOfFire)
	//	Debug.Log(ringOfFire);//take dmg
}

void GameManager::addBuildings()
{
	

	struct HeightMapAttributes
	{
		int Width_Columns;
		int Height_Row;
		std::vector<std::vector<DirectX::XMFLOAT3>> VertInfo;
	};
	HeightMapAttributes HeightMapVariables;
	FILE *tmpFile;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;
	unsigned char* bitMapImage_Info;
	int mapSize, bitMapIndx = 0, index;
	DirectX::XMFLOAT3 tempXMFLOAT3 = DirectX::XMFLOAT3(0, 0, 0);


	tmpFile = fopen("Assets/shouldwork.bmp", "rb");
	if (!tmpFile)
	{
		std::cout << "Error: Couldn't find HeightMap." << std::endl;
		
	}

	//Initiate BitMap Headers (File, Info).
	fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, tmpFile);
	fread(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, tmpFile);
	HeightMapVariables.Height_Row = InfoHeader.biHeight;
	HeightMapVariables.Width_Columns = InfoHeader.biWidth;

	//RGB size (r-byte, g-byte, b-byte).
	mapSize = HeightMapVariables.Height_Row * HeightMapVariables.Width_Columns * 3;

	//Store Image Data.
	bitMapImage_Info = new unsigned char[mapSize]; //Set fitting size for array that will contain data.
	fseek(tmpFile, FileHeader.bfOffBits, SEEK_SET); //Pointer is set to the beginning of data.
	fread(bitMapImage_Info, 1, mapSize, tmpFile); //Store data in bitMapImage_Info.
	fclose(tmpFile);
	int a = HeightMapVariables.Height_Row;
	int b = HeightMapVariables.Width_Columns;
	for (int i = 0; i < HeightMapVariables.Height_Row +1; i++)
	{
		std::vector<DirectX::XMFLOAT3> tempHeightVert;

		for (int j = 0; j < HeightMapVariables.Width_Columns +1; j++)
		{

			tempXMFLOAT3 = DirectX::XMFLOAT3((float)i, ((float)bitMapImage_Info[bitMapIndx] / 10.0f /*Smoothing Value.*/), (float)j);
			bitMapIndx += 3;
			tempHeightVert.push_back(tempXMFLOAT3);
		}

		HeightMapVariables.VertInfo.push_back(tempHeightVert);

	}
	



	//--------VVVVVV--------GOLDMINE---VVVVV---------------------
	int i = 50;
	int j = 25;

	GameObject* goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y - 3, j, 0));
	goldMineGameObject->name = "GoldMine";
	goldMineGameObject->tag = 0;
	Mesh* mesh = AssetManager.getMesh("QuarryTwo1_Mesh");
	MeshFilter* meshFilter2 = new MeshFilter(mesh);
	goldMineGameObject->addComponent(meshFilter2);
	goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldMineMaterial")));
	Unit *goldMine = new Unit(GoldMine);
	goldMineGameObject->addComponent(goldMine);
	gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	goldMineGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 250;
	j = 25;

	goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	goldMineGameObject->name = "GoldMine";
	goldMineGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	goldMineGameObject->addComponent(meshFilter2);
	goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldMineMaterial")));
	goldMine = new Unit(GoldMine);
	goldMineGameObject->addComponent(goldMine);
	gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	goldMineGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 250;
	j = 275;

	goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y - 3, j, 0));
	goldMineGameObject->name = "GoldMine";
	goldMineGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	goldMineGameObject->addComponent(meshFilter2);
	goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldMineMaterial")));
	goldMine = new Unit(GoldMine);
	goldMineGameObject->addComponent(goldMine);
	gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	goldMineGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 50;
	j = 275;

	goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y - 3, j, 0));
	goldMineGameObject->name = "GoldMine";
	goldMineGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	goldMineGameObject->addComponent(meshFilter2);
	goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldMineMaterial")));
	goldMine = new Unit(GoldMine);
	goldMineGameObject->addComponent(goldMine);
	gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	goldMineGameObject->raycastOption = RayCastOptions::DETAILED;

	//-------------------------------------------

	i = 125;
	j = 125;

	goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	goldMineGameObject->name = "GoldMine";
	goldMineGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	goldMineGameObject->addComponent(meshFilter2);
	goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldMineMaterial")));
	goldMine = new Unit(GoldMine);
	goldMineGameObject->addComponent(goldMine);
	gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	goldMineGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 175;
	j = 175;

	goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	goldMineGameObject->name = "GoldMine";
	goldMineGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	goldMineGameObject->addComponent(meshFilter2);
	goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldMineMaterial")));
	goldMine = new Unit(GoldMine);
	goldMineGameObject->addComponent(goldMine);
	gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	goldMineGameObject->raycastOption = RayCastOptions::DETAILED;

	//----------------------------------------

	//--------^^^^^^^^--------GOLDMINE---^^^^^^^^-------------------
	//--------VVVVVV--------Barrack---VVVVV---------------------
	i = 15;
	j = 10;
	GameObject* barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	barrackGameObject->name = "Barrack";
	barrackGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("CryptBarracksFinale1"));
	barrackGameObject->addComponent(meshFilter2);
	barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarracksMaterial")));
	Unit *barrack = new Unit(Barrack);
	barrackGameObject->addComponent(barrack);
	gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	barrackGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 285;
	j = 10;
	barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y + 1, j, 0));
	barrackGameObject->name = "Barrack";
	barrackGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("CryptBarracksFinale1"));
	barrackGameObject->addComponent(meshFilter2);
	barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarracksMaterial")));
	barrack = new Unit(Barrack);
	barrackGameObject->addComponent(barrack);
	gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	barrackGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 285;
	j = 290;
	barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y-1, j, 0));
	barrackGameObject->name = "Barrack";
	barrackGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("CryptBarracksFinale1"));
	barrackGameObject->addComponent(meshFilter2);
	barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarracksMaterial")));
	barrack = new Unit(Barrack);
	barrackGameObject->addComponent(barrack);
	gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	barrackGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 15;
	j = 290;
	barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y-1, j, 0));
	barrackGameObject->name = "Barrack";
	barrackGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("CryptBarracksFinale1"));
	barrackGameObject->addComponent(meshFilter2);
	barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarracksMaterial")));
	barrack = new Unit(Barrack);
	barrackGameObject->addComponent(barrack);
	gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	barrackGameObject->raycastOption = RayCastOptions::DETAILED;

	//-------------------------------------------

	i = 135;
	j = 215;
	barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y+ 1, j, 0));
	barrackGameObject->name = "Barrack";
	barrackGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("CryptBarracksFinale1"));
	barrackGameObject->addComponent(meshFilter2);
	barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarracksMaterial")));
	barrack = new Unit(Barrack);
	barrackGameObject->addComponent(barrack);
	gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	barrackGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 165;
	j = 75;
	barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y+2, j, 0));
	barrackGameObject->name = "Barrack";
	barrackGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("CryptBarracksFinale1"));
	barrackGameObject->addComponent(meshFilter2);
	barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarracksMaterial")));
	barrack = new Unit(Barrack);
	barrackGameObject->addComponent(barrack);
	gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	barrackGameObject->raycastOption = RayCastOptions::DETAILED;

	//----------------------------------------

	//--------^^^^^^^^--------Barrack---^^^^^^^^-------------------
	//--------VVVVVV--------BANK---VVVVV---------------------

	i = 25;
	j = 50;

	GameObject* bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	bankGameObject->name = "Bank";
	bankGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("FinalSilo1OBJ"));
	bankGameObject->addComponent(meshFilter2);
	bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("ResourceMaterial")));
	Unit *bank = new Unit(Bank);
	bankGameObject->addComponent(bank);
	gamemanager.buildingLists[bankGameObject->tag].push_back(bank);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	bankGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 275;
	j = 50;

	bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y + 1, j, 0));
	bankGameObject->name = "Bank";
	bankGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("FinalSilo1OBJ"));
	bankGameObject->addComponent(meshFilter2);
	bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("ResourceMaterial")));
	bank = new Unit(Bank);
	bankGameObject->addComponent(bank);
	gamemanager.buildingLists[bankGameObject->tag].push_back(bank);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	bankGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 275;
	j = 250;

	bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	bankGameObject->name = "Bank";
	bankGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("FinalSilo1OBJ"));
	bankGameObject->addComponent(meshFilter2);
	bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("ResourceMaterial")));
	bank = new Unit(Bank);
	bankGameObject->addComponent(bank);
	gamemanager.buildingLists[bankGameObject->tag].push_back(bank);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	bankGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 25;
	j = 250;

	bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	bankGameObject->name = "Bank";
	bankGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("FinalSilo1OBJ"));
	bankGameObject->addComponent(meshFilter2);
	bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("ResourceMaterial")));
	bank = new Unit(Bank);
	bankGameObject->addComponent(bank);
	gamemanager.buildingLists[bankGameObject->tag].push_back(bank);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	bankGameObject->raycastOption = RayCastOptions::DETAILED;

	//-------------------------------------------

	i = 120;
	j = 215;

	bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y + 1, j, 0));
	bankGameObject->name = "Bank";
	bankGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("FinalSilo1OBJ"));
	bankGameObject->addComponent(meshFilter2);
	bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("ResourceMaterial")));
	bank = new Unit(Bank);
	bankGameObject->addComponent(bank);
	gamemanager.buildingLists[bankGameObject->tag].push_back(bank);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	bankGameObject->raycastOption = RayCastOptions::DETAILED;

	i = 180;
	j = 80;

	bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y + 2, j, 0));
	bankGameObject->name = "Bank";
	bankGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("FinalSilo1OBJ"));
	bankGameObject->addComponent(meshFilter2);
	bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("ResourceMaterial")));
	bank = new Unit(Bank);
	bankGameObject->addComponent(bank);
	gamemanager.buildingLists[bankGameObject->tag].push_back(bank);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	bankGameObject->raycastOption = RayCastOptions::DETAILED;


	//Props
	////Tree
	i = 265;
	j = 234;
	i = (300 - i);
	j = (300 - j);
	GameObject* tree = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y+5, j, 0));
	Prop* treeProp = new Prop();
	tree->addComponent(treeProp);
	tree->name = "Tree";
	tree->tag = 0;
	tree->raycastOption = RayCastOptions::NONE;
	MeshFilter* meshFilterTree = new MeshFilter(AssetManager.getMesh("Spruce_Tree2"));
	tree->addComponent(meshFilterTree);
	tree->addComponent(new MaterialFilter(AssetManager.getMaterial("TreeMaterial")));

	i = 98;
	j = 210;
	i = (300 - i);
	j = (300 - j);
	tree = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	treeProp = new Prop();
	tree->addComponent(treeProp);
	tree->name = "Tree";
	tree->tag = 0;
	tree->raycastOption = RayCastOptions::NONE;
	meshFilterTree = new MeshFilter(AssetManager.getMesh("Spruce_Tree2"));
	tree->addComponent(meshFilterTree);
	tree->addComponent(new MaterialFilter(AssetManager.getMaterial("TreeMaterial")));

	i = 198;
	j = 96;
	i = (300 - i);
	j = (300 - j);
	tree = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	treeProp = new Prop();
	tree->addComponent(treeProp);
	tree->name = "Tree";
	tree->tag = 0;
	tree->raycastOption = RayCastOptions::NONE;
	meshFilterTree = new MeshFilter(AssetManager.getMesh("Spruce_Tree2"));
	tree->addComponent(meshFilterTree);
	tree->addComponent(new MaterialFilter(AssetManager.getMaterial("TreeMaterial")));

	i = 181;
	j = 100;
	i = (300 - i);
	j = (300 - j);
	tree = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	treeProp = new Prop();
	tree->addComponent(treeProp);
	tree->name = "Tree";
	tree->tag = 0;
	tree->raycastOption = RayCastOptions::NONE;
	meshFilterTree = new MeshFilter(AssetManager.getMesh("Spruce_Tree2"));
	tree->addComponent(meshFilterTree);
	tree->addComponent(new MaterialFilter(AssetManager.getMaterial("TreeMaterial")));

	i = 80;
	j = 98;
	i = (300 - i);
	j = (300 - j);
	tree = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	treeProp = new Prop();
	tree->addComponent(treeProp);
	tree->name = "Tree";
	tree->tag = 0;
	tree->raycastOption = RayCastOptions::NONE;
	meshFilterTree = new MeshFilter(AssetManager.getMesh("Spruce_Tree2"));
	tree->addComponent(meshFilterTree);
	tree->addComponent(new MaterialFilter(AssetManager.getMaterial("TreeMaterial")));

	i = 133;
	j = 166;
	i = (300 - i);
	j = (300 - j);
	tree = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y+5, j, 0));
	treeProp = new Prop();
	tree->addComponent(treeProp);
	tree->name = "Tree";
	tree->tag = 0;
	tree->raycastOption = RayCastOptions::NONE;
	meshFilterTree = new MeshFilter(AssetManager.getMesh("Spruce_Tree2"));
	tree->addComponent(meshFilterTree);
	tree->addComponent(new MaterialFilter(AssetManager.getMaterial("TreeMaterial")));

	//Flowers/Bushes
	i = 210;
	j = 236;
	i = (300 - i);
	j = (300 - j);
	GameObject* FlowersAndBushes = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	Prop* FlowersProp = new Prop();
	FlowersAndBushes->addComponent(FlowersProp);
	FlowersAndBushes->name = "FlowersAndBushes";
	FlowersAndBushes->tag = 0;
	FlowersAndBushes->raycastOption = RayCastOptions::NONE;
	MeshFilter* FlowerBushesFilter = new MeshFilter(AssetManager.getMesh("FlowersAndBushes4"));
	FlowersAndBushes->addComponent(FlowerBushesFilter);
	FlowersAndBushes->addComponent(new MaterialFilter(AssetManager.getMaterial("FlowersAndBushesMaterial")));


	i = 110;
	j = 136;
	i = (300 - i);
	j = (300 - j);
	FlowersAndBushes = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y+1, j, 0));
	FlowersProp = new Prop();
	FlowersAndBushes->addComponent(FlowersProp);
	FlowersAndBushes->name = "FlowersAndBushes";
	FlowersAndBushes->tag = 0;
	FlowersAndBushes->raycastOption = RayCastOptions::NONE;
	FlowerBushesFilter = new MeshFilter(AssetManager.getMesh("FlowersAndBushes4"));
	FlowersAndBushes->addComponent(FlowerBushesFilter);
	FlowersAndBushes->addComponent(new MaterialFilter(AssetManager.getMaterial("FlowersAndBushesMaterial")));

	i = 130;
	j = 216;
	i = (300 - i);
	j = (300 - j);
	FlowersAndBushes = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y+1, j, 0));
	FlowersProp = new Prop();
	FlowersAndBushes->addComponent(FlowersProp);
	FlowersAndBushes->name = "FlowersAndBushes";
	FlowersAndBushes->tag = 0;
	FlowersAndBushes->raycastOption = RayCastOptions::NONE;
	FlowerBushesFilter = new MeshFilter(AssetManager.getMesh("FlowersAndBushes4"));
	FlowersAndBushes->addComponent(FlowerBushesFilter);
	FlowersAndBushes->addComponent(new MaterialFilter(AssetManager.getMaterial("FlowersAndBushesMaterial")));

	i = 170;
	j = 136;
	i = (300 - i);
	j = (300 - j);
	FlowersAndBushes = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y+1, j, 0));
	FlowersProp = new Prop();
	FlowersAndBushes->addComponent(FlowersProp);
	FlowersAndBushes->name = "FlowersAndBushes";
	FlowersAndBushes->tag = 0;
	FlowersAndBushes->raycastOption = RayCastOptions::NONE;
	FlowerBushesFilter = new MeshFilter(AssetManager.getMesh("FlowersAndBushes4"));
	FlowersAndBushes->addComponent(FlowerBushesFilter);
	FlowersAndBushes->addComponent(new MaterialFilter(AssetManager.getMaterial("FlowersAndBushesMaterial")));

	i = 110;
	j = 206;
	i = (300 - i);
	j = (300 - j);
	FlowersAndBushes = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j+1, 0));
	FlowersProp = new Prop();
	FlowersAndBushes->addComponent(FlowersProp);
	FlowersAndBushes->name = "FlowersAndBushes";
	FlowersAndBushes->tag = 0;
	FlowersAndBushes->raycastOption = RayCastOptions::NONE;
	FlowerBushesFilter = new MeshFilter(AssetManager.getMesh("FlowersAndBushes4"));
	FlowersAndBushes->addComponent(FlowerBushesFilter);
	FlowersAndBushes->addComponent(new MaterialFilter(AssetManager.getMaterial("FlowersAndBushesMaterial")));

	//////Stairs
	////left
	//i = 149;
	//j = 117;
	//i = (300 - i);
	//j = (300 - j);
	//GameObject* stair1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y -2, j, 0));
	//Prop* stairProp1 = new Prop();
	//stair1->addComponent(stairProp1);
	//stair1->name = "Stair1";
	//stair1->tag = 0;
	//stair1->raycastOption = RayCastOptions::NONE;
	//MeshFilter* meshFilterStair1 = new MeshFilter(AssetManager.getMesh("Stairs"));
	//stair1->addComponent(meshFilterStair1);
	//stair1->addComponent(new MaterialFilter(AssetManager.getMaterial("StairsMaterial")));

	////right
	//i = 149;
	//j = 180;
	//i = (300 - i);
	//j = (300 - j);
	//GameObject* stair2 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y-2, j, 0));
	//Prop* stairProp2 = new Prop();
	//stair2->addComponent(stairProp2);
	//stair2->transform.setRotation(DirectX::XMVectorSet(0, 180, 0, 0));
	//stair2->name = "Stair2";
	//stair2->tag = 0;
	//stair2->raycastOption = RayCastOptions::NONE;
	//MeshFilter* meshFilterStair2 = new MeshFilter(AssetManager.getMesh("Stairs"));
	//stair2->addComponent(meshFilterStair2);
	//stair2->addComponent(new MaterialFilter(AssetManager.getMaterial("StairsMaterial")));

	////down
	//i = 118;
	//j = 152;
	//i = (300 - i);
	//j = (300 - j);
	//GameObject* stair3 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y-2, j, 0));
	//Prop* stairProp3 = new Prop();
	//stair3->addComponent(stairProp3);
	//stair3->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));
	//stair3->name = "stair3";
	//stair3->tag = 0;
	//stair3->raycastOption = RayCastOptions::NONE;
	//MeshFilter* meshFilterstair3 = new MeshFilter(AssetManager.getMesh("Stairs"));
	//stair3->addComponent(meshFilterstair3);
	//stair3->addComponent(new MaterialFilter(AssetManager.getMaterial("StairsMaterial")));

	////up
	//i = 178;
	//j = 149;
	//i = (300 - i);
	//j = (300 - j);
	//GameObject* stair4 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y-2, j, 0));
	//Prop* stairProp4 = new Prop();
	//stair4->addComponent(stairProp4);
	//stair4->transform.setRotation(DirectX::XMVectorSet(0, 270, 0, 0));
	//stair4->name = "stair4";
	//stair4->tag = 0;
	//stair4->raycastOption = RayCastOptions::NONE;
	//MeshFilter* meshFilterstair4 = new MeshFilter(AssetManager.getMesh("Stairs"));
	//stair4->addComponent(meshFilterstair4);
	//stair4->addComponent(new MaterialFilter(AssetManager.getMaterial("StairsMaterial")));

	//Ferns
	i = 268;
	j = 234;
	i = (300 - i);
	j = (300 - j);
	GameObject* fern1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	Prop* fernProp1 = new Prop();
	fern1->addComponent(fernProp1);
	fern1->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));
	fern1->name = "fern1";
	fern1->tag = 0;
	fern1->raycastOption = RayCastOptions::NONE;
	MeshFilter* meshFilterfern1 = new MeshFilter(AssetManager.getMesh("Fern"));
	fern1->addComponent(meshFilterfern1);
	fern1->addComponent(new MaterialFilter(AssetManager.getMaterial("FernMaterial")));

	i = 238;
	j = 264;
	i = (300 - i);
	j = (300 - j);
	fern1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	fernProp1 = new Prop();
	fern1->addComponent(fernProp1);
	fern1->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));
	fern1->name = "fern1";
	fern1->tag = 0;
	fern1->raycastOption = RayCastOptions::NONE;
	meshFilterfern1 = new MeshFilter(AssetManager.getMesh("Fern"));
	fern1->addComponent(meshFilterfern1);
	fern1->addComponent(new MaterialFilter(AssetManager.getMaterial("FernMaterial")));

	i = 238;
	j = 164;
	i = (300 - i);
	j = (300 - j);
	fern1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	fernProp1 = new Prop();
	fern1->addComponent(fernProp1);
	fern1->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));
	fern1->name = "fern1";
	fern1->tag = 0;
	fern1->raycastOption = RayCastOptions::NONE;
	meshFilterfern1 = new MeshFilter(AssetManager.getMesh("Fern"));
	fern1->addComponent(meshFilterfern1);
	fern1->addComponent(new MaterialFilter(AssetManager.getMaterial("FernMaterial")));

	i = 138;
	j = 264;
	i = (300 - i);
	j = (300 - j);
	fern1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	fernProp1 = new Prop();
	fern1->addComponent(fernProp1);
	fern1->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));
	fern1->name = "fern1";
	fern1->tag = 0;
	fern1->raycastOption = RayCastOptions::NONE;
	meshFilterfern1 = new MeshFilter(AssetManager.getMesh("Fern"));
	fern1->addComponent(meshFilterfern1);
	fern1->addComponent(new MaterialFilter(AssetManager.getMaterial("FernMaterial")));

	i = 228;
	j = 54;
	i = (300 - i);
	j = (300 - j);
	fern1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	fernProp1 = new Prop();
	fern1->addComponent(fernProp1);
	fern1->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));
	fern1->name = "fern1";
	fern1->tag = 0;
	fern1->raycastOption = RayCastOptions::NONE;
	meshFilterfern1 = new MeshFilter(AssetManager.getMesh("Fern"));
	fern1->addComponent(meshFilterfern1);
	fern1->addComponent(new MaterialFilter(AssetManager.getMaterial("FernMaterial")));

	i = 38;
	j = 64;
	i = (300 - i);
	j = (300 - j);
	fern1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	fernProp1 = new Prop();
	fern1->addComponent(fernProp1);
	fern1->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));
	fern1->name = "fern1";
	fern1->tag = 0;
	fern1->raycastOption = RayCastOptions::NONE;
	meshFilterfern1 = new MeshFilter(AssetManager.getMesh("Fern"));
	fern1->addComponent(meshFilterfern1);
	fern1->addComponent(new MaterialFilter(AssetManager.getMaterial("FernMaterial")));

	//Lion Pillars
	i = 192;
	j = 222;
	i = (300 - i);
	j = (300 - j);
	GameObject* lionPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	Prop* lionPillarProp1 = new Prop();
	lionPillar1->addComponent(lionPillarProp1);
	lionPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	lionPillar1->name = "lionPillar1";
	lionPillar1->tag = 0;
	lionPillar1->raycastOption = RayCastOptions::NONE;
	MeshFilter* meshFilterlionPillar1 = new MeshFilter(AssetManager.getMesh("LionPillar"));
	lionPillar1->addComponent(meshFilterlionPillar1);
	lionPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("LionPillarMaterial")));

	i = 52;
	j = 142;
	i = (300 - i);
	j = (300 - j);
	lionPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	lionPillarProp1 = new Prop();
	lionPillar1->addComponent(lionPillarProp1);
	lionPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	lionPillar1->name = "lionPillar1";
	lionPillar1->tag = 0;
	lionPillar1->raycastOption = RayCastOptions::NONE;
	meshFilterlionPillar1 = new MeshFilter(AssetManager.getMesh("LionPillar"));
	lionPillar1->addComponent(meshFilterlionPillar1);
	lionPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("LionPillarMaterial")));

	i = 192;
	j = 192;
	i = (300 - i);
	j = (300 - j);
	lionPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	lionPillarProp1 = new Prop();
	lionPillar1->addComponent(lionPillarProp1);
	lionPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	lionPillar1->name = "lionPillar1";
	lionPillar1->tag = 0;
	lionPillar1->raycastOption = RayCastOptions::NONE;
	meshFilterlionPillar1 = new MeshFilter(AssetManager.getMesh("LionPillar"));
	lionPillar1->addComponent(meshFilterlionPillar1);
	lionPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("LionPillarMaterial")));

	i = 152;
	j = 52;
	i = (300 - i);
	j = (300 - j);
	lionPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	lionPillarProp1 = new Prop();
	lionPillar1->addComponent(lionPillarProp1);
	lionPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	lionPillar1->name = "lionPillar1";
	lionPillar1->tag = 0;
	lionPillar1->raycastOption = RayCastOptions::NONE;
	meshFilterlionPillar1 = new MeshFilter(AssetManager.getMesh("LionPillar"));
	lionPillar1->addComponent(meshFilterlionPillar1);
	lionPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("LionPillarMaterial")));

	i = 52;
	j = 252;
	i = (300 - i);
	j = (300 - j);
	lionPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	lionPillarProp1 = new Prop();
	lionPillar1->addComponent(lionPillarProp1);
	lionPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	lionPillar1->name = "lionPillar1";
	lionPillar1->tag = 0;
	lionPillar1->raycastOption = RayCastOptions::NONE;
	meshFilterlionPillar1 = new MeshFilter(AssetManager.getMesh("LionPillar"));
	lionPillar1->addComponent(meshFilterlionPillar1);
	lionPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("LionPillarMaterial")));

	//Ruined Pillars
	i = 120;
	j = 120;
	i = (300 - i);
	j = (300 - j);
	GameObject* ruinedPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	Prop* ruinedPillarProp1 = new Prop();
	ruinedPillar1->addComponent(ruinedPillarProp1);
	ruinedPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	ruinedPillar1->name = "ruinedPillar1";
	ruinedPillar1->tag = 0;
	ruinedPillar1->raycastOption = RayCastOptions::NONE;
	MeshFilter* meshFilterRuinedPillar1 = new MeshFilter(AssetManager.getMesh("RuinedPillar"));
	ruinedPillar1->addComponent(meshFilterRuinedPillar1);
	ruinedPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("RuinedPillarMaterial")));

	i = 140;
	j = 90;
	i = (300 - i);
	j = (300 - j);
	ruinedPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	ruinedPillarProp1 = new Prop();
	ruinedPillar1->addComponent(ruinedPillarProp1);
	ruinedPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	ruinedPillar1->name = "ruinedPillar2";
	ruinedPillar1->tag = 0;
	ruinedPillar1->raycastOption = RayCastOptions::NONE;
	meshFilterRuinedPillar1 = new MeshFilter(AssetManager.getMesh("RuinedPillar"));
	ruinedPillar1->addComponent(meshFilterRuinedPillar1);
	ruinedPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("RuinedPillarMaterial")));

	i = 70;
	j = 120;
	i = (300 - i);
	j = (300 - j);
	ruinedPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	ruinedPillarProp1 = new Prop();
	ruinedPillar1->addComponent(ruinedPillarProp1);
	ruinedPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	ruinedPillar1->name = "ruinedPillar3";
	ruinedPillar1->tag = 0;
	ruinedPillar1->raycastOption = RayCastOptions::NONE;
	meshFilterRuinedPillar1 = new MeshFilter(AssetManager.getMesh("RuinedPillar"));
	ruinedPillar1->addComponent(meshFilterRuinedPillar1);
	ruinedPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("RuinedPillarMaterial")));

	i = 240;
	j = 120;
	i = (300 - i);
	j = (300 - j);
	ruinedPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	ruinedPillarProp1 = new Prop();
	ruinedPillar1->addComponent(ruinedPillarProp1);
	ruinedPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	ruinedPillar1->name = "ruinedPillar4";
	ruinedPillar1->tag = 0;
	ruinedPillar1->raycastOption = RayCastOptions::NONE;
	meshFilterRuinedPillar1 = new MeshFilter(AssetManager.getMesh("RuinedPillar"));
	ruinedPillar1->addComponent(meshFilterRuinedPillar1);
	ruinedPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("RuinedPillarMaterial")));

	i = 100;
	j = 240;
	i = (300 - i);
	j = (300 - j);
	ruinedPillar1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	ruinedPillarProp1 = new Prop();
	ruinedPillar1->addComponent(ruinedPillarProp1);
	ruinedPillar1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	ruinedPillar1->name = "ruinedPillar5";
	ruinedPillar1->tag = 0;
	ruinedPillar1->raycastOption = RayCastOptions::NONE;
	meshFilterRuinedPillar1 = new MeshFilter(AssetManager.getMesh("RuinedPillar"));
	ruinedPillar1->addComponent(meshFilterRuinedPillar1);
	ruinedPillar1->addComponent(new MaterialFilter(AssetManager.getMaterial("RuinedPillarMaterial")));

	//Brazier
	i = 80;
	j = 120;
	i = (300 - i);
	j = (300 - j);
	GameObject* brazier1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	Prop* brazier1Prop1 = new Prop();
	brazier1->addComponent(brazier1Prop1);
	brazier1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	brazier1->name = "brazier1";
	brazier1->tag = 0;
	brazier1->raycastOption = RayCastOptions::NONE;
	MeshFilter* meshFilterbrazier1 = new MeshFilter(AssetManager.getMesh("Brazier"));
	brazier1->addComponent(meshFilterbrazier1);
	brazier1->addComponent(new MaterialFilter(AssetManager.getMaterial("BrazierMaterial")));

	i = 80;
	j = 120;
	i = (300 - i);
	j = (300 - j);
	brazier1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	brazier1Prop1 = new Prop();
	brazier1->addComponent(brazier1Prop1);
	brazier1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	brazier1->name = "brazier1";
	brazier1->tag = 0;
	brazier1->raycastOption = RayCastOptions::NONE;
	meshFilterbrazier1 = new MeshFilter(AssetManager.getMesh("Brazier"));
	brazier1->addComponent(meshFilterbrazier1);
	brazier1->addComponent(new MaterialFilter(AssetManager.getMaterial("BrazierMaterial")));

	i = 150;
	j = 270;
	i = (300 - i);
	j = (300 - j);
	brazier1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	brazier1Prop1 = new Prop();
	brazier1->addComponent(brazier1Prop1);
	brazier1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	brazier1->name = "brazier1";
	brazier1->tag = 0;
	brazier1->raycastOption = RayCastOptions::NONE;
	meshFilterbrazier1 = new MeshFilter(AssetManager.getMesh("Brazier"));
	brazier1->addComponent(meshFilterbrazier1);
	brazier1->addComponent(new MaterialFilter(AssetManager.getMaterial("BrazierMaterial")));

	i = 210;
	j = 150;
	i = (300 - i);
	j = (300 - j);
	brazier1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	brazier1Prop1 = new Prop();
	brazier1->addComponent(brazier1Prop1);
	brazier1->transform.setRotation(DirectX::XMVectorSet(0, -90, 0, 0));
	brazier1->name = "brazier1";
	brazier1->tag = 0;
	brazier1->raycastOption = RayCastOptions::NONE;
	meshFilterbrazier1 = new MeshFilter(AssetManager.getMesh("Brazier"));
	brazier1->addComponent(meshFilterbrazier1);
	brazier1->addComponent(new MaterialFilter(AssetManager.getMaterial("BrazierMaterial")));

	//----------------------------------------

	//--------^^^^^^^^--------BANK---^^^^^^^^-------------------
	/*i = 25;
	j = 250;

	goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	goldMineGameObject->name = "GoldMine";
	goldMineGameObject->tag = 0;
	meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	goldMineGameObject->addComponent(meshFilter2);
	goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
	goldMine = new Unit(GoldMine);
	goldMineGameObject->addComponent(goldMine);
	gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	 i = 50;
	 j = 25;

	 goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y- 5, j, 0));
	 goldMineGameObject->name = "Goldmine";
	 goldMineGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	 goldMineGameObject->addComponent(meshFilter2);
	 goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
	 goldMine = new Unit(GoldMine);
	 goldMineGameObject->addComponent(goldMine);
	 gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	 PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	 i = 50;
	 j = 275;

	 goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y-5, j, 0));
	 goldMineGameObject->name = "Bank";
	 goldMineGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
	 goldMineGameObject->addComponent(meshFilter2);
	 goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
	 goldMine = new Unit(Bank);
	 goldMineGameObject->addComponent(goldMine);
	 gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	 PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));

	 i = 250;
	 j = 25;

	 goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 goldMineGameObject->name = "Goldmine";
	 goldMineGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	 goldMineGameObject->addComponent(meshFilter2);
	 goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
	 goldMine = new Unit(GoldMine);
	 goldMineGameObject->addComponent(goldMine);
	 gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	 PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	 i = 250;
	 j = 275;

	 goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 goldMineGameObject->name = "Bank";
	 goldMineGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
	 goldMineGameObject->addComponent(meshFilter2);
	 goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
	 goldMine = new Unit(Bank);
	 goldMineGameObject->addComponent(goldMine);
	 gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	 PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	 i = 275;
	 j = 50;

	 goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 goldMineGameObject->name = "Bank";
	 goldMineGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
	 goldMineGameObject->addComponent(meshFilter2);
	 goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
	 goldMine = new Unit(Bank);
	 goldMineGameObject->addComponent(goldMine);
	 gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	 PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
	 i = 275;
	 j = 250;

	 goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 goldMineGameObject->name = "Goldmine";
	 goldMineGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
	 goldMineGameObject->addComponent(meshFilter2);
	 goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
	 goldMine = new Unit(GoldMine);
	 goldMineGameObject->addComponent(goldMine);
	 gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	 PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));


	 i = 25;
	 j = 25;
	 GameObject* barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 barrackGameObject->name = "Barrack";
	 barrackGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("BarracksTest1"));
	 barrackGameObject->addComponent(meshFilter2);
	 barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarrackMaterial")));
	 Unit *barrack = new Unit(Barrack);
	 barrackGameObject->addComponent(barrack);
	 gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);

	 i = 275;
	 j = 275;
	 barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 barrackGameObject->name = "Barrack";
	 barrackGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("BarracksTest1"));
	 barrackGameObject->addComponent(meshFilter2);
	 barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarrackMaterial")));
	 barrack = new Unit(Barrack);
	 barrackGameObject->addComponent(barrack);
	 gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);

	 i = 25;
	 j = 275;
	 barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 barrackGameObject->name = "Barrack";
	 barrackGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("BarracksTest1"));
	 barrackGameObject->addComponent(meshFilter2);
	 barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarrackMaterial")));
	 barrack = new Unit(Barrack);
	 barrackGameObject->addComponent(barrack);
	 gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);

	 i = 275;
	 j = 25;
	 barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 barrackGameObject->name = "Barrack";
	 barrackGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("BarracksTest1"));
	 barrackGameObject->addComponent(meshFilter2);
	 barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarrackMaterial")));
	 barrack = new Unit(Barrack);
	 barrackGameObject->addComponent(barrack);
	 gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);

	 i = 10;
	 j = 75;

	 GameObject* bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	 bankGameObject->name = "Bank";
	 bankGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
	 bankGameObject->addComponent(meshFilter2);
	 bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
	 Unit *bank = new Unit(Bank);
	 bankGameObject->addComponent(bank);
	 gamemanager.buildingLists[bankGameObject->tag].push_back(bank);

	 i = 75;
	 j = 10;

	 bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y - 5, j, 0));
	 bankGameObject->name = "Bank";
	 bankGameObject->tag = 0;
	 meshFilter2 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
	 bankGameObject->addComponent(meshFilter2);
	 bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
	 bank = new Unit(Bank);
	 bankGameObject->addComponent(bank);
	 gamemanager.buildingLists[bankGameObject->tag].push_back(bank);*/

	
	/*int x = 0;
	for (int i = 25; i < HeightMapVariables.VertInfo[0].size(); i += 50)
	{
		
		if(i < 100 || i > 200)
		for (int j = 25; j < HeightMapVariables.VertInfo.size(); j += 50)
		{
			if (j < 100 || j > 200)
			{
				if (x == 0)
				{
					GameObject* goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
					goldMineGameObject->name = "Goldmine";
					goldMineGameObject->tag = 0;
					MeshFilter* meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
					goldMineGameObject->addComponent(meshFilter2);
					goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
					Unit *goldMine = new Unit(GoldMine);
					goldMineGameObject->addComponent(goldMine);
					gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
					PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));
					x++;
					continue;
				}
				
				if (x == 1)
				{
					GameObject* bankGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
					bankGameObject->name = "Bank";
					bankGameObject->tag = 0;
					MeshFilter* meshFilter2 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
					bankGameObject->addComponent(meshFilter2);
					bankGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
					Unit *bank = new Unit(Bank);
					bankGameObject->addComponent(bank);
					gamemanager.buildingLists[bankGameObject->tag].push_back(bank);
					x++;
					continue;
				}
				if (x == 2)
				{
					GameObject* barrackGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y +2, j, 0));
					barrackGameObject->name = "Barrack";
					barrackGameObject->tag = 0;
					MeshFilter* meshFilter2 = new MeshFilter(AssetManager.getMesh("BarracksTest1"));
					barrackGameObject->addComponent(meshFilter2);
					barrackGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("BarrackMaterial")));
					Unit *barrack = new Unit(Barrack);
					barrackGameObject->addComponent(barrack);
					gamemanager.buildingLists[barrackGameObject->tag].push_back(barrack);
					x = 0;
					continue;
				}
			
			
			}
			
		}
	}*/

	delete[] bitMapImage_Info;
	bitMapImage_Info = 0;

	/*for (int i = 0; i < 275; i += 50)
	{
		for (int j = 25; j < 275; j += 50)
		{
			GameObject* goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, 15, j, 0));
			goldMineGameObject->name = "Goldmine";
			goldMineGameObject->tag = 0;
			MeshFilter* meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
			goldMineGameObject->addComponent(meshFilter2);
			goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
			Unit *goldMine = new Unit(GoldMine);
			goldMineGameObject->addComponent(goldMine);
			gamemanager.unitLists[0].push_back(goldMine);
		}
	}*/
	//GameObject* cube3 = gScene.createEmptyGameObject(DirectX::XMVectorSet(5, 0, 30, 0));
	//cube3->name = "Bank";
	//cube3->tag = 0;
	//MeshFilter* meshFilter3 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
	//cube3->addComponent(meshFilter3);
	//cube3->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
	//Unit* unitBuilding = new Unit(Bank);
	//cube3->addComponent(unitBuilding);
	//playerscript->friendlyBuildings.push_back(unitBuilding);
	//unitBuilding->setPlayerScript(playerscript);

	//GameObject* barrack = gScene.createEmptyGameObject(DirectX::XMVectorSet(30, 0, 5, 0));
	//barrack->name = "Barrack";
	//barrack->tag = 1;
	//MeshFilter* meshFilterBarracks = new MeshFilter(AssetManager.getMesh("BarracksTest1"));
	//barrack->addComponent(meshFilterBarracks);
	//barrack->addComponent(new MaterialFilter(AssetManager.getMaterial("BarrackMaterial")));
	//Unit* unitBuilding2 = new Unit(Barrack);
	//barrack->addComponent(unitBuilding2);
	//playerscript->friendlyBuildings.push_back(unitBuilding2);
	//unitBuilding2->setPlayerScript(playerscript);
}
