#include "GameManager.h"




std::vector<std::vector<Unit*>> GameManager::unitLists;
std::vector<std::vector<Unit*>> GameManager::buildingLists;
GAME_STATE GameManager::gameState;


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

	gameState = GAME_STATE::START_STATE;
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
	gDevice->CreateBuffer(&ManagerDesc, &InitData, &GameManagerBuffer);
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
		break;
	case START_STATE:
		addBuildings();
		gameState = LARGE_CIRCLE_STATE;
		break;
	case LARGE_CIRCLE_STATE:
		if (gameTime >= 120)
		{
			//Debug.Log(ringOfFire);  
			ringOfFire -= 0.02f * Time.getDeltaTime() * gameTime;
		}
		if (ringOfFire < 150) {
			gameState = GAME_STATE::MEDIUM_CIRCLE_STATE;
			lavaSpeed = 0.01;
		}
		dmgRing();
		break;
	case MEDIUM_CIRCLE_STATE:
		if (gameTime >= 180)
		{
			//Debug.Log(ringOfFire);  
			//devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &ringOfFire, 0, 0);
			ringOfFire -= 0.02f * Time.getDeltaTime() * gameTime;
			lavaSpeed = 0.05;
		}
		if (ringOfFire < 80) {
			gameState = GAME_STATE::SMALL_CIRCLE_STATE;
			lavaSpeed = 0.01;
		}
		dmgRing();
		break;
	case SMALL_CIRCLE_STATE:
		if (gameTime >= 240 && ringOfFire > 28)
		{
			Debug.Log(ringOfFire);  
			//devCon->UpdateSubresource(GameManagerBuffer, 0, nullptr, &ringOfFire, 0, 0);
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
	int i = 25;
	int j = 50;

	GameObject* goldMineGameObject = gScene.createEmptyGameObject(DirectX::XMVectorSet(i, HeightMapVariables.VertInfo[i][j].y, j, 0));
	goldMineGameObject->name = "GoldMine";
	goldMineGameObject->tag = 0;
	Mesh* mesh = AssetManager.getMesh("QuarryTwo1_Mesh");
	MeshFilter* meshFilter2 = new MeshFilter(mesh);
	goldMineGameObject->addComponent(meshFilter2);
	goldMineGameObject->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
	Unit *goldMine = new Unit(GoldMine);
	goldMineGameObject->addComponent(goldMine);
	gamemanager.buildingLists[goldMineGameObject->tag].push_back(goldMine);
	PathCreator.blockGrid(DirectX::XMFLOAT3(i, HeightMapVariables.VertInfo[i][j].y, j));

	i = 25;
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
	 gamemanager.buildingLists[bankGameObject->tag].push_back(bank);

	
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
