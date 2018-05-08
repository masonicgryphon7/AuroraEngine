#include "CoreEngine.h"
#include "GUI.h"
#include "GUI_Viewport.h"
#include "GUI_MenuBar.h"
#include "GUI_Console.h"
#include "GUI_Hierarchy.h"
#include "GUI_Inspector.h"
#include "Console.h"
#include "imgui_dock.h"
#include "ImGuizmo.h"
#include "Editor.h"
#include "Player.h"
#include "NPC.h"
#include "Debug.h"
#include "GameManager.h"
#include <crtdbg.h>
#include "PathCreator.h"
#pragma comment(lib, "dxgi.lib")

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 
#define GRAPHICS_DEBUGGER_ENABLED 1
#define PLAYER_BUILD 1

bool CoreEngine::hasResized = false;

std::wstring s2ws(const std::string& s);

CoreEngine::CoreEngine(bool includeDebugConsole)
{
	if (includeDebugConsole)
	{
		if (!AllocConsole())
			MessageBox(0, L"Couldn't launch debug console!", 0, 0);
		else
		{
			consoleInitialized = true;
			//SetWindowPos(GetConsoleWindow(), 0, 500, 500, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			freopen("CONOUT$", "w", stdout);
			std::cout << " -- CONSOLE LAUNCHED -- " << std::endl << std::endl << std::endl;
		}
	}
}

CoreEngine::~CoreEngine()
{
	if (consoleInitialized)
	{
		if (!FreeConsole())
			MessageBox(0, L"Couldn't turn off debug console!", 0, 0);
	}

	SAFE_RELEASE(m_alphaDisabledBlendState);
	SAFE_RELEASE(m_alphaEnableBlendState);
	SAFE_RELEASE(m_depthStencilBuffer);
	SAFE_RELEASE(m_depthStencilState);
	//SAFE_RELEASE(m_depthStencilStateDisabled);
	//SAFE_RELEASE(m_depthStencilStateEnabled);
	SAFE_RELEASE(m_depthStencilView);
	//SAFE_RELEASE(m_rasterizerState);
	SAFE_RELEASE(m_rasterStateCullBack);
	SAFE_RELEASE(m_rasterStateCullFront);
	SAFE_RELEASE(m_rasterStateCullNone);
	SAFE_RELEASE(gBackbufferRTV);
	SAFE_RELEASE(gVertexBuffer);
	SAFE_RELEASE(textureview);
	SAFE_RELEASE(gVertexLayout);
	SAFE_RELEASE(gSwapChain);

	Input.~InputHandler();
	gScene.~Scene();
	AssetManager.~cAssetManager();
	SAFE_RELEASE(gDeviceContext);
	SAFE_RELEASE(gDevice);
	// Cleanup
	//gSwapChain->SetFullscreenState(FALSE, NULL);
	/*gBackbufferRTV->Release();
	gSwapChain->Release();
	gDevice->Release();*/

	//m_alphaEnableBlendState->Release();
	//m_alphaDisabledBlendState->Release();
	////m_rasterizerState->Release();
	//m_depthStencilState->Release();
	////m_depthStencilStateDisabled->Release();
	////m_depthStencilStateEnabled->Release();
	//m_depthStencilBuffer->Release();
	//m_depthStencilView->Release();
	///*gBackbufferRTV->Release();
	//gDevice->Release();
	//gDeviceContext->Release();
	//gSwapChain->Release();*/
}

MSG CoreEngine::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); //1. Skapa f�nster

	this->wnd = wndHandle;

	if (wndHandle)
	{
		if (!PLAYER_BUILD)
			CreateDirect3DContext(wndHandle);
		else
			CreatePlayerDirect3DContext(wndHandle);

		SetViewport(); //3. S�tt viewport

		CreateTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		ShowWindow(wndHandle, SW_MAXIMIZE);//nCmdShow);
		UpdateWindow(wndHandle);
		SetFocus(wndHandle);

		// IMGUI

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		ImGui_ImplDX11_Init(wndHandle, gDevice, gDeviceContext);
		io.MouseDrawCursor = true;
		//io.SetCustomMouseTexture = false;

		//ImGui_ImplWin32_UpdateMouseCursor();

		Time.start();

		renderManager = new RenderManager(gDevice, gDeviceContext, gBackbufferRTV, gSwapChain, m_depthStencilView);
		if (PLAYER_BUILD)
			renderManager->CreateRenderTarget(WIDTH, HEIGHT);

		InputHandler inputHandler = InputHandler(wndHandle); // this has memory leak

		AssetManager.Start(gDevice, gDeviceContext);

		//assetManager = cAssetManager(gDevice, gDeviceContext); // this has memory leak
		AssetManager.addShaderProgram(INPUT_ELEMENT_DESCRIPTION::INPUT_ELEMENT_POS3UV2T3B3N3, "Vertex.hlsl", SHADER_TYPE::VERTEX_SHADER);
		AssetManager.addShaderProgram(INPUT_ELEMENT_DESCRIPTION::INPUT_ELEMENT_POS3UV2T3B3N3JNT4WT4, "VertexAnimation.hlsl", SHADER_TYPE::VERTEX_SHADER);

		AssetManager.addShaderProgram("Fragment.hlsl", SHADER_TYPE::PIXEL_SHADER);


		//shaderProgram.CreateShaderData(gDeviceContext, gDevice, descTest, "Vertex.hlsl", "", "", "", "Fragment.hlsl", "");


		addMaterials();

		//----------------
		GameManager gameManager = GameManager(gDevice, gDeviceContext);

		createTerrain();
		
		//Meshes
		AssetManager.addMeshFromBinary("Assets/PIRATE.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/BarracksTest1.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.addMeshFromBinary("Assets/QuarryTwo1_Mesh.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Spruce_Tree2.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Test2ResourceSilo.obj", AssetManager.getShaderProgram("Vertex.hlsl"));

	

		//PathCreator.createNodes();

		// Create a Main Camera
		Camera* cam = nullptr;
		camera = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 70, 0, 0)); //(DirectX::XMVectorSet(0, 25, 0, 0));
		camera->name = "Main Camera";
		cam = new Camera(HEIGHT, WIDTH, 35, 0.01f, 1000.0f);
		camera->transform.setRotation(DirectX::XMVectorSet(0, 0, 70, 0)); //(DirectX::XMVectorSet(0, 0, 70, 0));
		camera->addComponent(cam);
		PlayerScript* playerscript = new PlayerScript(camera);
		camera->addComponent(playerscript);

		//PlayerScript *playerscript = new PlayerScript();
		//camera->addComponent(playerscript);

		//Tree
		GameObject* tree = gScene.createEmptyGameObject(DirectX::XMVectorSet(7, 0, 20, 0));
		tree->name = "Tree";
		tree->tag = 0;
		MeshFilter* meshFilterTree = new MeshFilter(AssetManager.getMesh("Spruce_Tree2"));
		tree->addComponent(meshFilterTree);
		tree->addComponent(new MaterialFilter(AssetManager.getMaterial("TreeMaterial")));

		//Barrack



		AssetManager.addMeshFromBinary("Assets/COLLECTOR.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.addMeshFromBinary("Assets/pose1smile.bin", AssetManager.getShaderProgram("Vertex.hlsl"));


		AudioListener* audioListener = new AudioListener();
		camera->addComponent(audioListener);

		playerscript->instantiate_Player();

		//GameObject* cube = gScene.createEmptyGameObject(DirectX::XMVectorSet(1, 0, 1, 0));
		//cube->name = "Worker";
		//cube->tag = 1;
		//MeshFilter* meshFilter1 = new MeshFilter(AssetManager.getMesh("pose1smile"));
		//cube->addComponent(meshFilter1);
		//cube->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));
		//Unit *unitWorker = new Unit(Worker);
		//cube->addComponent(unitWorker);
		////playerscript->friendlyUnits.push_back(unitWorker);
		//unitWorker->setPlayerScript(playerscript);
		//gamemanager.unitLists[cube->tag].push_back(unitWorker);


		GameObject* cube2 = gScene.createEmptyGameObject(DirectX::XMVectorSet(40, 0, 5, 0));
		cube2->name = "Goldmine";
		cube2->tag = 0;
		MeshFilter* meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
		cube2->addComponent(meshFilter2);
		cube2->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
		Unit *goldMine = new Unit(GoldMine);
		cube2->addComponent(goldMine);
		goldMine->setPlayerScript(playerscript);

		GameObject* cube3 = gScene.createEmptyGameObject(DirectX::XMVectorSet(5, 0, 30, 0));
		cube3->name = "Bank";
		cube3->tag = 0;
		MeshFilter* meshFilter3 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
		cube3->addComponent(meshFilter3);
		cube3->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
		Unit* unitBuilding = new Unit(Bank);
		cube3->addComponent(unitBuilding);
		//playerscript->friendlyBuildings.push_back(unitBuilding);
		unitBuilding->setPlayerScript(playerscript);
		gamemanager.buildingLists[cube3->tag].push_back(unitBuilding);

		GameObject* barrack = gScene.createEmptyGameObject(DirectX::XMVectorSet(30, 0, 5, 0));
		barrack->name = "Barrack";
		barrack->tag = 1;
		MeshFilter* meshFilterBarracks = new MeshFilter(AssetManager.getMesh("BarracksTest1"));
		barrack->addComponent(meshFilterBarracks);
		barrack->addComponent(new MaterialFilter(AssetManager.getMaterial("BarrackMaterial")));
		Unit* unitBuilding2 = new Unit(Barrack);
		barrack->addComponent(unitBuilding2);
		//playerscript->friendlyBuildings.push_back(unitBuilding2);
		unitBuilding2->setPlayerScript(playerscript);
		gamemanager.buildingLists[barrack->tag].push_back(unitBuilding2);


		//
		GameObject* animatedGO = gScene.createEmptyGameObject(DirectX::XMVectorSet(5, 0, 5, 0));
		animatedGO->name = "Animator";
		AssetManager.addAnimatedMeshFromBinary("Assets/pCube1_ANIMATION_Mesh.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		Mesh* animMesh = AssetManager.getMesh("pCube1_ANIMATION_Mesh");
		MeshFilter* animMeshFilter = new MeshFilter(animMesh);
		animatedGO->addComponent(animMeshFilter);

		AssetManager.addSkeletonFromBinary("Assets/First_JOint_Skeleton.bin");
		Animator* animator = new Animator(assetManager.getSkeleton("First_JOint_Skeleton"));
		animatedGO->addComponent(animator);

		AssetManager.addAnimationClipFromBinary(assetManager.getSkeleton("First_JOint_Skeleton"), "Assets/ANIMATION_ANIMATION.bin");
		animator->addAnimationClip(AssetManager.getAnimationclip(assetManager.getSkeleton("First_JOint_Skeleton"), "ANIMATION_ANIMATION"));

		animatedGO->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));

		//


		GameObject* enemy_player = gScene.createEmptyGameObject();
		NPC* enemy_NPC = new NPC();
		enemy_NPC->instantiate_NPC();
		enemy_player->addComponent(enemy_NPC);





		/*ClickToMove* clickToMove = new ClickToMove(cam);
		cube->addComponent(clickToMove);*/




		Editor* editor = nullptr;
		Player* player = nullptr;

		if (!PLAYER_BUILD)
		{
			editor = new Editor();
			editor->Start(&wndHandle, gDevice, gDeviceContext, this);
		}
		else
		{
			player = new Player();
			player->Start(&wndHandle, gDevice, gDeviceContext, this);
			gSwapChain->SetFullscreenState(TRUE, NULL);
		}

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				inputHandler.updateInput();
				Time.tick();
				gameManager.update();
				if (!PLAYER_BUILD)
					OnResize();

				gScene.destroyGameObjects();
				gScene.update();
				gScene.frustumCull(camera);
				objectsToRender = gScene.getFrustumCulledResult();

				if (editor != nullptr)
					editor->Update();
				if (player != nullptr)
					player->Update();


				if (!PLAYER_BUILD)
					gDeviceContext->PSSetShaderResources(0, 1, &renderManager->m_shaderResourceView);

				renderManager->EndFrame(); // END RENDERING

				objectsToRender[0].clear();

				hasResized = false;
			}
		}

		gSwapChain->SetFullscreenState(FALSE, NULL);

		if (editor != nullptr)
			delete editor;

		if (player != nullptr)
			delete player;

		delete renderManager;
		DestroyWindow(wndHandle);
	}
	return msg;
}

void CoreEngine::CreateTriangleData()
{
	// Array of Structs (AoS)
	VERTEX_POS3UV2 triangleVertices[6] =
	{
		DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f),	//v0 pos
		DirectX::XMFLOAT2(1.0f, 1.0f), 	//v0 color

		DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f),	//v1
		DirectX::XMFLOAT2(1.0f, 0.0f), 	//v1 color

		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), //v2
		DirectX::XMFLOAT2(0.0f, 0.0f), 	//v2 color


		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),	//v0 pos
		DirectX::XMFLOAT2(0.0f, 0.0f), 	//v0 color

		DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f),	//v1
		DirectX::XMFLOAT2(0.0f, 1.0f), 	//v1 color

		DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f), //v2
		DirectX::XMFLOAT2(1.0f, 1.0f), 	//v2 color
	};

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;

	// create a Vertex Buffer
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);

}

void CoreEngine::addMaterials()
{


	//Terrain Texture.
	assetManager.addTexture("Assets/Grass_Albedo.png"); //3
	assetManager.addTexture("Assets/Grass_Normal.png"); //4
	assetManager.addTexture("Assets/Grass_AoMetalRough.png"); //5

	assetManager.addTexture("Assets/Cliff_Albedo.png"); //6
	assetManager.addTexture("Assets/Cliff_Normal.png"); //7
	assetManager.addTexture("Assets/Cliff_AoMetalRough.png"); //8

	assetManager.addTexture("Assets/Sand_Albedo.png"); //9
	assetManager.addTexture("Assets/Sand_Normal.png"); //10
	assetManager.addTexture("Assets/Sand_AoMetalRough.png"); //11

	assetManager.addTexture("Assets/ID_MAP2.png"); //12

	assetManager.addTexture("Assets/ID_MAP2part1.png"); // 16 // 1 // ALL THE PARTS SHOULD BE ROTATED CLOCKEWISE 90 DEGREES TO MATCH WITH BMP.
	assetManager.addTexture("Assets/ID_MAP2part2.png"); // 17 // 2
	assetManager.addTexture("Assets/ID_MAP2part3.png"); // 18 // 3
	assetManager.addTexture("Assets/ID_MAP2part4.png"); //19 // 4

	assetManager.addTexture("Assets/Lava_Albedo2.png"); //20
	assetManager.addTexture("Assets/Lava_Normal.png"); //21
	assetManager.addTexture("Assets/Lava_OcclusionRoughnessMetallic.png"); //22

																		   //Terrain
	int matXTile = 10;
	int matYTile = 10;
	//Terrain Material 1
	assetManager.AddMaterial("TerrainMaterial1", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("TerrainMaterial1")->setIsTerrain(true);
	assetManager.getMaterial("TerrainMaterial1")->setXTile(matXTile);
	assetManager.getMaterial("TerrainMaterial1")->setYTile(matYTile);
	assetManager.getMaterial("TerrainMaterial1")->setTerrainMaterials(
		assetManager.getTexture("Grass_Albedo")->getTexture(),
		assetManager.getTexture("Grass_Normal")->getTexture(),
		assetManager.getTexture("Grass_AoMetalRough")->getTexture(),
		assetManager.getTexture("Cliff_Albedo")->getTexture(),
		assetManager.getTexture("Cliff_Normal")->getTexture(),
		assetManager.getTexture("Cliff_AoMetalRough")->getTexture(),
		assetManager.getTexture("Sand_Albedo")->getTexture(),
		assetManager.getTexture("Sand_Normal")->getTexture(),
		assetManager.getTexture("Sand_AoMetalRough")->getTexture(),
		assetManager.getTexture("Lava_Albedo2")->getTexture(),
		assetManager.getTexture("Lava_Normal")->getTexture(),
		assetManager.getTexture("Lava_OcclusionRoughnessMetallic")->getTexture(),
		assetManager.getTexture("ID_MAP2part1")->getTexture()); //USE ID_PART 1

																//Terrain Material 2
	assetManager.AddMaterial("TerrainMaterial2", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("TerrainMaterial2")->setIsTerrain(true);
	assetManager.getMaterial("TerrainMaterial2")->setXTile(matXTile);
	assetManager.getMaterial("TerrainMaterial2")->setYTile(matYTile);
	assetManager.getMaterial("TerrainMaterial2")->setTerrainMaterials(
		assetManager.getTexture("Grass_Albedo")->getTexture(),
		assetManager.getTexture("Grass_Normal")->getTexture(),
		assetManager.getTexture("Grass_AoMetalRough")->getTexture(),
		assetManager.getTexture("Cliff_Albedo")->getTexture(),
		assetManager.getTexture("Cliff_Normal")->getTexture(),
		assetManager.getTexture("Cliff_AoMetalRough")->getTexture(),
		assetManager.getTexture("Sand_Albedo")->getTexture(),
		assetManager.getTexture("Sand_Normal")->getTexture(),
		assetManager.getTexture("Sand_AoMetalRough")->getTexture(),
		assetManager.getTexture("Lava_Albedo2")->getTexture(),
		assetManager.getTexture("Lava_Normal")->getTexture(),
		assetManager.getTexture("Lava_OcclusionRoughnessMetallic")->getTexture(),
		assetManager.getTexture("ID_MAP2part2")->getTexture()); //USE ID_PART 2

																//Terrain Material 1
	assetManager.AddMaterial("TerrainMaterial3", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("TerrainMaterial3")->setIsTerrain(true);
	assetManager.getMaterial("TerrainMaterial3")->setXTile(matXTile);
	assetManager.getMaterial("TerrainMaterial3")->setYTile(matYTile);
	assetManager.getMaterial("TerrainMaterial3")->setTerrainMaterials(
		assetManager.getTexture("Grass_Albedo")->getTexture(),
		assetManager.getTexture("Grass_Normal")->getTexture(),
		assetManager.getTexture("Grass_AoMetalRough")->getTexture(),
		assetManager.getTexture("Cliff_Albedo")->getTexture(),
		assetManager.getTexture("Cliff_Normal")->getTexture(),
		assetManager.getTexture("Cliff_AoMetalRough")->getTexture(),
		assetManager.getTexture("Sand_Albedo")->getTexture(),
		assetManager.getTexture("Sand_Normal")->getTexture(),
		assetManager.getTexture("Sand_AoMetalRough")->getTexture(),
		assetManager.getTexture("Lava_Albedo2")->getTexture(),
		assetManager.getTexture("Lava_Normal")->getTexture(),
		assetManager.getTexture("Lava_OcclusionRoughnessMetallic")->getTexture(),
		assetManager.getTexture("ID_MAP2part3")->getTexture()); //USE ID_PART 3

																//Terrain Material 4
	assetManager.AddMaterial("TerrainMaterial4", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("TerrainMaterial4")->setIsTerrain(true);
	assetManager.getMaterial("TerrainMaterial4")->setXTile(matXTile);
	assetManager.getMaterial("TerrainMaterial4")->setYTile(matYTile);
	assetManager.getMaterial("TerrainMaterial4")->setTerrainMaterials(
		assetManager.getTexture("Grass_Albedo")->getTexture(),
		assetManager.getTexture("Grass_Normal")->getTexture(),
		assetManager.getTexture("Grass_AoMetalRough")->getTexture(),
		assetManager.getTexture("Cliff_Albedo")->getTexture(),
		assetManager.getTexture("Cliff_Normal")->getTexture(),
		assetManager.getTexture("Cliff_AoMetalRough")->getTexture(),
		assetManager.getTexture("Sand_Albedo")->getTexture(),
		assetManager.getTexture("Sand_Normal")->getTexture(),
		assetManager.getTexture("Sand_AoMetalRough")->getTexture(),
		assetManager.getTexture("Lava_Albedo2")->getTexture(),
		assetManager.getTexture("Lava_Normal")->getTexture(),
		assetManager.getTexture("Lava_OcclusionRoughnessMetallic")->getTexture(),
		assetManager.getTexture("ID_MAP2part4")->getTexture()); //USE ID_PART 4


																//Unit & Tree textures
	AssetManager.addTexture("Assets/STSP_ShadowTeam_BaseColor.png");
	AssetManager.addTexture("Assets/STSP_ShadowTeam_Normal.png");
	AssetManager.addTexture("Assets/STSP_ShadowTeam_OcclusionRoughnessMetallic.png");


	AssetManager.addTexture("Assets/Spruce_Tree1_initialShadingGroup_BaseColor.png");
	AssetManager.addTexture("Assets/Spruce_Tree1_initialShadingGroup_Normal.png");
	AssetManager.addTexture("Assets/Spruce_Tree1_initialShadingGroup_OcclusionRoughnessMetallic.png");

	//Unit Material
	assetManager.AddMaterial("WorkerMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("WorkerMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("WorkerMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("WorkerMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());

	//Soldier material
	assetManager.AddMaterial("SoldierMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("SoldierMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("SoldierMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("SoldierMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());

	//Hero material
	assetManager.AddMaterial("HeroMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("HeroMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("HeroMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("HeroMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());

	//Bank material
	assetManager.AddMaterial("BankMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("BankMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("BankMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("BankMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());

	//Goldmine material
	assetManager.AddMaterial("GoldmineMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("GoldmineMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("GoldmineMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("GoldmineMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());

	//Barrack material
	assetManager.AddMaterial("BarrackMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("BarrackMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("BarrackMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("BarrackMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());


	//Tree Material
	assetManager.AddMaterial("TreeMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("TreeMaterial")->setAlbedo(assetManager.getTexture("Spruce_Tree1_initialShadingGroup_BaseColor")->getTexture());
	assetManager.getMaterial("TreeMaterial")->setNormal(assetManager.getTexture("Spruce_Tree1_initialShadingGroup_Normal")->getTexture());
	assetManager.getMaterial("TreeMaterial")->setAORoughMet(assetManager.getTexture("Spruce_Tree1_initialShadingGroup_OcclusionRoughnessMetallic")->getTexture());

}

void CoreEngine::createTerrain()
{
	GameObject* terrain1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain1->name = "Terrain1";
	terrain1->tag = 0;
	terrain1->detailedRaycast = true;
	TerrainGenerator* terrainGenerator1 = new TerrainGenerator(100, 100, "Assets/7.bmp"); //BMP SKA VARA 298 I STORLEK.
	AssetManager.addMesh(terrainGenerator1->vertCount, &terrainGenerator1->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain = new MeshFilter(AssetManager.getMesh(0));
	terrain1->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial3")));
	terrain1->addComponent(meshFilterTerrain);

	GameObject* terrain2 = gScene.createEmptyGameObject(DirectX::XMVectorSet(99, 0, 0, 0));
	terrain2->name = "Terrain2";
	terrain2->tag = 0;
	terrain2->detailedRaycast = true;
	TerrainGenerator* terrainGenerator2 = new TerrainGenerator(100, 100, "Assets/4.bmp");
	AssetManager.addMesh(terrainGenerator2->vertCount, &terrainGenerator2->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain2 = new MeshFilter(AssetManager.getMesh(1));
	terrain2->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain2->addComponent(meshFilterTerrain2);

	GameObject* terrain3 = gScene.createEmptyGameObject(DirectX::XMVectorSet(198, 0, 0, 0));
	terrain3->name = "Terrain3";
	terrain3->tag = 0;
	terrain3->detailedRaycast = true;
	TerrainGenerator* terrainGenerator3 = new TerrainGenerator(100, 100, "Assets/1.bmp");
	AssetManager.addMesh(terrainGenerator3->vertCount, &terrainGenerator3->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain3 = new MeshFilter(AssetManager.getMesh(2));
	terrain3->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial4")));
	terrain3->addComponent(meshFilterTerrain3);

	GameObject* terrain4 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 99, 0));
	terrain4->name = "Terrain4";
	terrain4->tag = 0;
	terrain4->detailedRaycast = true;
	TerrainGenerator* terrainGenerator4 = new TerrainGenerator(100, 100, "Assets/8.bmp");
	AssetManager.addMesh(terrainGenerator4->vertCount, &terrainGenerator4->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain4 = new MeshFilter(AssetManager.getMesh(3));
	terrain4->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial2")));
	terrain4->addComponent(meshFilterTerrain4);

	GameObject* terrain5 = gScene.createEmptyGameObject(DirectX::XMVectorSet(99, 0, 99, 0));
	terrain5->name = "Terrain5";
	terrain5->tag = 0;
	terrain5->detailedRaycast = true;
	TerrainGenerator* terrainGenerator5 = new TerrainGenerator(100, 100, "Assets/5.bmp");
	AssetManager.addMesh(terrainGenerator5->vertCount, &terrainGenerator5->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain5 = new MeshFilter(AssetManager.getMesh(4));
	terrain5->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial2")));
	terrain5->addComponent(meshFilterTerrain5);

	GameObject* terrain6 = gScene.createEmptyGameObject(DirectX::XMVectorSet(198, 0, 99, 0));
	terrain6->name = "Terrain6";
	terrain6->tag = 0;
	terrain6->detailedRaycast = true;
	TerrainGenerator* terrainGenerator6 = new TerrainGenerator(100, 100, "Assets/2.bmp");
	AssetManager.addMesh(terrainGenerator6->vertCount, &terrainGenerator6->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain6 = new MeshFilter(AssetManager.getMesh(5));
	terrain6->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial2")));
	terrain6->addComponent(meshFilterTerrain6);

	GameObject* terrain7 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 198, 0));
	terrain7->name = "Terrain7";
	terrain7->tag = 0;
	terrain7->detailedRaycast = true;
	TerrainGenerator* terrainGenerator7 = new TerrainGenerator(100, 100, "Assets/9.bmp");
	AssetManager.addMesh(terrainGenerator7->vertCount, &terrainGenerator7->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain7 = new MeshFilter(AssetManager.getMesh(6));
	terrain7->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial2")));
	terrain7->addComponent(meshFilterTerrain7);

	GameObject* terrain8 = gScene.createEmptyGameObject(DirectX::XMVectorSet(99, 0, 198, 0));
	terrain8->name = "Terrain8";
	terrain8->tag = 0;
	terrain8->detailedRaycast = true;
	TerrainGenerator* terrainGenerator8 = new TerrainGenerator(100, 100, "Assets/6.bmp");
	AssetManager.addMesh(terrainGenerator8->vertCount, &terrainGenerator8->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain8 = new MeshFilter(AssetManager.getMesh(7));
	terrain8->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial2")));
	terrain8->addComponent(meshFilterTerrain8);

	GameObject* terrain9 = gScene.createEmptyGameObject(DirectX::XMVectorSet(198, 0, 198, 0));
	terrain9->name = "Terrain9";
	terrain9->tag = 0;
	terrain9->detailedRaycast = true;
	TerrainGenerator* terrainGenerator9 = new TerrainGenerator(100, 100, "Assets/3.bmp");
	AssetManager.addMesh(terrainGenerator9->vertCount, &terrainGenerator9->TriangleArr, AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain9 = new MeshFilter(AssetManager.getMesh(8));
	terrain9->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial2")));
	terrain9->addComponent(meshFilterTerrain9);
	//PathCreator.createNodes(terrainGenerator1->getRealVertArr());
	cPathCreator* PathCreator1 = new cPathCreator(300, 300); // 200x200

	PathCreator1->addTerrain(terrainGenerator1->getRealVertArr(), 0, 0);
	PathCreator1->addTerrain(terrainGenerator2->getRealVertArr(), 0, 99);
	PathCreator1->addTerrain(terrainGenerator3->getRealVertArr(), 0, 198);
	PathCreator1->addTerrain(terrainGenerator4->getRealVertArr(), 99, 0);
	PathCreator1->addTerrain(terrainGenerator5->getRealVertArr(), 99, 99);
	PathCreator1->addTerrain(terrainGenerator6->getRealVertArr(), 99, 198);
	PathCreator1->addTerrain(terrainGenerator7->getRealVertArr(), 198, 0);
	PathCreator1->addTerrain(terrainGenerator8->getRealVertArr(), 198, 99);
	PathCreator1->addTerrain(terrainGenerator9->getRealVertArr(), 198, 198);

		PathCreator.trumpTheBorders();

		//PathCreator.createNodes();

		// Create a Main Camera
		Camera* cam = nullptr;
		camera = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 35, 0, 0)); //(DirectX::XMVectorSet(0, 25, 0, 0));
		camera->name = "Main Camera";
		cam = new Camera(HEIGHT, WIDTH, 70.0f, 0.01f, 1000.0f);
		camera->transform.setRotation(DirectX::XMVectorSet(0, 0, 70, 0)); //(DirectX::XMVectorSet(0, 0, 70, 0));
		camera->addComponent(cam);
		PlayerScript* playerscript = new PlayerScript(camera);
		camera->addComponent(playerscript);

		//PlayerScript *playerscript = new PlayerScript();
		//camera->addComponent(playerscript);

		//Tree
		GameObject* tree = gScene.createEmptyGameObject(DirectX::XMVectorSet(7, 0, 20, 0));
		tree->name = "Tree";
		tree->tag = 0;
		MeshFilter* meshFilterTree = new MeshFilter(AssetManager.getMesh("Spruce_Tree2"));
		tree->addComponent(meshFilterTree);
		tree->addComponent(new MaterialFilter(AssetManager.getMaterial("TreeMaterial")));

		//Barrack



		AssetManager.addMeshFromBinary("Assets/COLLECTOR.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
	

		AudioListener* audioListener = new AudioListener();
		camera->addComponent(audioListener);

		playerscript->instantiate_Player();

		//GameObject* cube4 = gScene.createEmptyGameObject(DirectX::XMVectorSet(20, 0, 10, 0));
		//cube4->name = "Hero";
		//cube4->tag = 1;
		//MeshFilter* meshFilter4 = new MeshFilter(AssetManager.getMesh("PIRATE"));
		//cube4->addComponent(meshFilter4);
		//cube4->addComponent(new MaterialFilter(AssetManager.getMaterial("HeroMaterial")));
		//Unit* UnitHero = new Unit(Hero);
		//cube4->addComponent(UnitHero);
		//playerscript->friendlyUnits.push_back(UnitHero);
		//UnitHero->setPlayerScript(playerscript);
		//playerscript->friendlyUnits.at(0)->setHomePos(&playerscript->friendlyBuildings.at(0)->gameObject->transform);
		//gamemanager.unitLists[cube4->tag].push_back(UnitHero);

		GameObject* cube = gScene.createEmptyGameObject(DirectX::XMVectorSet(1, 0, 1, 0));
		cube->name = "Worker";
		cube->tag = 1;
		AssetManager.addMeshFromBinary("Assets/pose1smile.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		MeshFilter* meshFilter1 = new MeshFilter(AssetManager.getMesh("pose1smile"));
		cube->addComponent(meshFilter1);
		cube->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));
		Unit *unitWorker = new Unit(Worker);
		cube->addComponent(unitWorker);
		//playerscript->friendlyUnits.push_back(unitWorker);
		unitWorker->setPlayerScript(playerscript);
		gamemanager.unitLists[cube->tag].push_back(unitWorker);


		GameObject* cube2 = gScene.createEmptyGameObject(DirectX::XMVectorSet(40, 0, 5, 0));
		cube2->name = "Goldmine";
		cube2->tag = 0;
		MeshFilter* meshFilter2 = new MeshFilter(AssetManager.getMesh("QuarryTwo1_Mesh"));
		cube2->addComponent(meshFilter2);
		cube2->addComponent(new MaterialFilter(AssetManager.getMaterial("GoldmineMaterial")));
		Unit *goldMine = new Unit(GoldMine);
		cube2->addComponent(goldMine);
		goldMine->setPlayerScript(playerscript);

		GameObject* cube3 = gScene.createEmptyGameObject(DirectX::XMVectorSet(5, 0, 30, 0));
		cube3->name = "Bank";
		cube3->tag = 0;
		MeshFilter* meshFilter3 = new MeshFilter(AssetManager.getMesh("Test2ResourceSilo"));
		cube3->addComponent(meshFilter3);
		cube3->addComponent(new MaterialFilter(AssetManager.getMaterial("BankMaterial")));
		Unit* unitBuilding = new Unit(Bank);
		cube3->addComponent(unitBuilding);
		//playerscript->friendlyBuildings.push_back(unitBuilding);
		unitBuilding->setPlayerScript(playerscript);
		gamemanager.buildingLists[cube3->tag].push_back(unitBuilding);

		GameObject* barrack = gScene.createEmptyGameObject(DirectX::XMVectorSet(30, 0, 5, 0));
		barrack->name = "Barrack";
		barrack->tag = 1;
		MeshFilter* meshFilterBarracks = new MeshFilter(AssetManager.getMesh("BarracksTest1"));
		barrack->addComponent(meshFilterBarracks);
		barrack->addComponent(new MaterialFilter(AssetManager.getMaterial("BarrackMaterial")));
		Unit* unitBuilding2 = new Unit(Barrack);
		barrack->addComponent(unitBuilding2);
		//playerscript->friendlyBuildings.push_back(unitBuilding2);
		unitBuilding2->setPlayerScript(playerscript);
		gamemanager.buildingLists[barrack->tag].push_back(unitBuilding2);


		//
		GameObject* animatedGO = gScene.createEmptyGameObject(DirectX::XMVectorSet(5, 0, 5, 0));
		animatedGO->name = "Animator";
		AssetManager.addAnimatedMeshFromBinary("Assets/pCube1_ANIMATION_Mesh.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		Mesh* animMesh = AssetManager.getMesh("pCube1_ANIMATION_Mesh");
		MeshFilter* animMeshFilter = new MeshFilter(animMesh);
		animatedGO->addComponent(animMeshFilter);

		AssetManager.addSkeletonFromBinary("Assets/First_JOint_Skeleton.bin");
		Animator* animator = new Animator(assetManager.getSkeleton("First_JOint_Skeleton"));
		animatedGO->addComponent(animator);
		
		AssetManager.addAnimationClipFromBinary(assetManager.getSkeleton("First_JOint_Skeleton"),"Assets/ANIMATION_ANIMATION.bin");
		animator->addAnimationClip(AssetManager.getAnimationclip(assetManager.getSkeleton("First_JOint_Skeleton"),"ANIMATION_ANIMATION"));

		animatedGO->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));

		//
		

		GameObject* enemy_player = gScene.createEmptyGameObject();
		NPC* enemy_NPC = new NPC();
		enemy_NPC->instantiate_NPC();
		enemy_player->addComponent(enemy_NPC);
		




		/*ClickToMove* clickToMove = new ClickToMove(cam);
		cube->addComponent(clickToMove);*/




		Editor* editor = nullptr;
		Player* player = nullptr;

		if (!PLAYER_BUILD)
		{
			editor = new Editor();
			editor->Start(&wndHandle, gDevice, gDeviceContext, this);
		}
		else
		{
			player = new Player();
			player->Start(&wndHandle, gDevice, gDeviceContext, this);
			gSwapChain->SetFullscreenState(TRUE, NULL);
		}

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				inputHandler.updateInput();
				Time.tick();
				gameManager.update();
				if (!PLAYER_BUILD)
					OnResize();

				gScene.destroyGameObjects();
				gScene.update();
				gScene.frustumCull(camera);
				objectsToRender = gScene.getFrustumCulledResult();

				if (editor != nullptr)
					editor->Update();
				if (player != nullptr)
					player->Update();


				if (!PLAYER_BUILD)
					gDeviceContext->PSSetShaderResources(0, 1, &renderManager->m_shaderResourceView);

				renderManager->EndFrame(); // END RENDERING

				objectsToRender[0].clear();

				hasResized = false;
			}
		}

		gSwapChain->SetFullscreenState(FALSE, NULL);

		if (editor != nullptr)
			delete editor;

		if (player != nullptr)
			delete player;

		delete renderManager;
		DestroyWindow(wndHandle);
	}
	return msg;
}

void CoreEngine::CreateTriangleData()
{
	// Array of Structs (AoS)
	VERTEX_POS3UV2 triangleVertices[6] =
	{
		DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f),	//v0 pos
		DirectX::XMFLOAT2(1.0f, 1.0f), 	//v0 color

		DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f),	//v1
		DirectX::XMFLOAT2(1.0f, 0.0f), 	//v1 color

		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), //v2
		DirectX::XMFLOAT2(0.0f, 0.0f), 	//v2 color


		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),	//v0 pos
		DirectX::XMFLOAT2(0.0f, 0.0f), 	//v0 color

		DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f),	//v1
		DirectX::XMFLOAT2(0.0f, 1.0f), 	//v1 color

		DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f), //v2
		DirectX::XMFLOAT2(1.0f, 1.0f), 	//v2 color
	};

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;

	// create a Vertex Buffer
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);

}

void CoreEngine::SetViewport(int x, int y)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)x;
	vp.Height = (float)y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
}

void CoreEngine::OnResize()
{
	if (!GRAPHICS_DEBUGGER_ENABLED)
	{
		if (!hasResized)
			return;

		hasResized = false;

		Vector2 ns = Input.GetEngineWindowResolution();
		Vector2 sns = Input.GetDesktopResolution();

		Input.GetEngineWindowResolution();

		int width = Input.GetWidth();
		int height = Input.GetHeight();

		gBackbufferRTV->Release();
		gBackbufferRTV = nullptr;
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;

		gSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		ID3D11Texture2D* pBuffer;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

		gDevice->CreateRenderTargetView(pBuffer, NULL, &gBackbufferRTV);

		pBuffer->Release();

		gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, NULL);

		SetViewport(sns.x, sns.y);

		CreateDepthStencilBuffer();
		CreateDepthStencilView();

		renderManager->UpdateStuff(gDevice, gDeviceContext, gBackbufferRTV, gSwapChain, m_depthStencilView);

		renderManager->CreateRenderTarget(ns.x, ns.y);

		ImGui_ImplDX11_InvalidateDeviceObjects();
		ImGui_ImplDX11_CreateDeviceObjects();
	}
	else
	{
		if (!hasResized)
			return;

		Vector2 ns = Input.GetEngineWindowResolution();

		gBackbufferRTV->Release();
		gBackbufferRTV = nullptr;
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;

		DXGI_MODE_DESC dxgiModeDesc;
		ZeroMemory(&dxgiModeDesc, sizeof(dxgiModeDesc));
		dxgiModeDesc.Width = (unsigned int)ns.x;
		dxgiModeDesc.Height = (unsigned int)ns.y;
		dxgiModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiModeDesc.RefreshRate = DXGI_RATIONAL{ m_refreshRateNumerator, m_refreshRateDenominator };
		dxgiModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		HRESULT result = gSwapChain->ResizeTarget(&dxgiModeDesc);
		if (FAILED(result))
			Console.error("Failed to set resize swapchain");

		result = gSwapChain->ResizeBuffers(1, (unsigned int)ns.x, (unsigned int)ns.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		if (FAILED(result))
			Console.error("Failed to set resize buffer swapchain");

		ID3D11Texture2D* backBuffer = nullptr;
		result = gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBuffer));
		if (FAILED(result))
			Console.error("Failed to get buffer swapchain");

		result = gDevice->CreateRenderTargetView(backBuffer, nullptr, &gBackbufferRTV);

		backBuffer->Release();
		backBuffer = nullptr;

		if (FAILED(result))
			Console.error("Failed to create render target view.");

		CreateDepthStencilBuffer();
		CreateDepthStencilView();

		renderManager->UpdateStuff(gDevice, gDeviceContext, gBackbufferRTV, gSwapChain, m_depthStencilView);

		renderManager->CreateRenderTarget(ns.x, ns.y);

		ImGui_ImplDX11_InvalidateDeviceObjects();
		ImGui_ImplDX11_CreateDeviceObjects();
	}
}

string editorName = "Aurora Engine";
string gameName = "AE: RTS Game";

HWND CoreEngine::InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc = { 0 };
	/*wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"dSXOv9vbM1MIm5kAf4yjjw";

	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;*/

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"dSXOv9vbM1MIm5kAf4yjjw";
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
		return false;

	int windowWidth = GetSystemMetrics(SM_CXSCREEN);
	int windowHeight = GetSystemMetrics(SM_CYSCREEN);

	int x = WIDTH, y = HEIGHT;

	if (PLAYER_BUILD)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		x = desktop.right;
		y = desktop.bottom;
	}

	RECT rc = { 0, 0, x, y };
	//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	std::wstring stemp = s2ws(PLAYER_BUILD ? gameName : editorName);
	LPCWSTR result = stemp.c_str();

	HWND handle = CreateWindow(
		L"dSXOv9vbM1MIm5kAf4yjjw",
		result,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, // spawn window x
		CW_USEDEFAULT, // spawn window y
		rc.right - rc.left, // size x
		rc.bottom - rc.top, // size y
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

// https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
// Able to convert string to LPCWSTR which means we can insert whatever name we want into a handle window creation event
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

#define ID_NEW_SCENE 1001
#define ID_LOAD_SCENE 1002
#define ID_SAVE_SCENE 1003

#define ID_NEW_EMPTY_GAMEOBJECT 2001
#define ID_CREATE_CUBE 2002

#define ID_HELP_MEME 3001
#define ID_WORK_AMBITIONS 3002

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CoreEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
	{
		if (!PLAYER_BUILD)
		{
			HMENU hMenubar = CreateMenu();
			HMENU hFile = CreateMenu();
			HMENU hEdit = CreateMenu();
			HMENU hAssets = CreateMenu();
			HMENU hGameObject = CreateMenu();
			HMENU h3DObjects = CreatePopupMenu();
			HMENU hComponent = CreateMenu();
			HMENU hWindow = CreateMenu();
			HMENU hHelp = CreateMenu();

			//Creates menus
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, L"&File");
			AppendMenu(hMenubar, MF_POPUP, NULL, L"&Edit");
			AppendMenu(hMenubar, MF_POPUP, NULL, L"&Assets");
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hGameObject, L"&GameObject");
			AppendMenu(hMenubar, MF_POPUP, NULL, L"&Component");
			AppendMenu(hMenubar, MF_POPUP, NULL, L"&Window");
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hHelp, L"&Help");

			AppendMenu(hFile, MF_STRING, ID_NEW_SCENE, L"New Scene");
			AppendMenu(hFile, MF_STRING, ID_LOAD_SCENE, L"Open Scene");
			AppendMenu(hFile, MF_SEPARATOR, NULL, L"Separator");
			AppendMenu(hFile, MF_STRING, ID_SAVE_SCENE, L"Save Scene");
			AppendMenu(hFile, MF_STRING, NULL, L"Save Scene as...");

			AppendMenu(hGameObject, MF_STRING, ID_NEW_EMPTY_GAMEOBJECT, L"Create Empty");
			AppendMenu(hGameObject, MF_STRING | MF_POPUP, (UINT_PTR)h3DObjects, L"3D Objects");
			AppendMenu(h3DObjects, MF_STRING, ID_CREATE_CUBE, L"Cube");
			//AppendMenu(hGameObject, MF_SEPARATOR, NULL, L"Separator");


			AppendMenu(hHelp, MF_STRING, ID_HELP_MEME, L"Help, I need some MEMES");
			AppendMenu(hHelp, MF_STRING, ID_WORK_AMBITIONS, L"I need some work help");


			SetMenu(hWnd, hMenubar);
		}
	}
	break;

	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_NEW_SCENE:
			Debug.Log("New Scene");
			break;

		case ID_LOAD_SCENE:
			gScene.LoadScene();
			break;

		case ID_SAVE_SCENE:
			gScene.SaveScene();
			break;

		case ID_NEW_EMPTY_GAMEOBJECT:
			gScene.CreateGameObject(Primitives::Empty);
			break;

		case ID_CREATE_CUBE:
			gScene.CreateGameObject(AssetManager.getMesh("Cube"));
			break;

		case ID_HELP_MEME:
			ShellExecute(0, 0, L"https://www.instagram.com/salad.snake/?hl=en", 0, 0, SW_SHOW);
			break;

		case ID_WORK_AMBITIONS:
			ShellExecute(0, 0, L"https://www.youtube.com/watch?v=-6_alv0z3mw", 0, 0, SW_SHOW);
			break;
		}
	}break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	{
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
	}
	break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_SIZE:
		hasResized = true;
		break;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = WIDTH;
		lpMMI->ptMinTrackSize.y = HEIGHT;
	}
	break;

	case WM_SETCURSOR:
		ShowCursor(message);
		break;
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT CoreEngine::CreateDirect3DContext(HWND wndHandle)
{
	HRESULT result;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	DXGI_MODE_DESC* displayModeList;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory));
	result = factory->EnumAdapters(0, &adapter);
	result = adapter->EnumOutputs(0, &adapterOutput);
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	Console.print("NumModes: ", numModes);

	displayModeList = new DXGI_MODE_DESC[numModes];
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

	for (int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)WIDTH)
		{
			if (displayModeList[i].Height == (unsigned int)HEIGHT)
			{
				Console.print("Numerator: ", displayModeList[i].RefreshRate.Numerator);
				Console.print("Denominator: ", displayModeList[i].RefreshRate.Denominator);
				m_refreshRateNumerator = displayModeList[i].RefreshRate.Numerator;
				m_refreshRateDenominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = nullptr;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = nullptr;

	// Release the adapter.
	adapter->Release();
	adapter = nullptr;

	// Release the factory.
	factory->Release();
	factory = nullptr;

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1; //was 4                               // how many multisamples
	scd.SampleDesc.Quality = 0; //new
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	UINT deviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif													// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);



	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, nullptr, &gBackbufferRTV);
		pBackBuffer->Release();

		pBackBuffer = nullptr;

		// set the render target as the back buffer
		gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, NULL);
	}

	SetViewport();

	/*if (!CreateDepthStencilState(m_depthStencilStateEnabled, true, true))
	Console.error("ERROR on DEPTH-ENABLED");

	if (!CreateDepthStencilState(m_depthStencilStateDisabled, false, false))
	Console.error("ERROR on DEPTH-DISABLED");*/

	Console.print("Is it still resizing?   ", hasResized);

	CreateDepthStencilBuffer();

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = gDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
		Console.error("Couldn't set depth stencil state");

	// Set the depth stencil state.
	gDeviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	CreateDepthStencilView();



	//gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, m_depthStencilView);



	if (!CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, &m_rasterStateCullBack))
		Console.error("Couldn't create raster-cull-back");

	if (!CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, &m_rasterStateCullFront))
		Console.error("Couldn't create raster-cull-front");

	if (!CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, &m_rasterStateCullNone))
		Console.error("Couldn't create raster-cull-none");

	gDeviceContext->RSSetState(m_rasterStateCullBack);

	SetViewport();

	D3D11_BLEND_DESC blendStateDescription;

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = gDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendState);
	if (FAILED(result))
		Console.error("Error Enable Blend State");

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	result = gDevice->CreateBlendState(&blendStateDescription, &m_alphaDisabledBlendState);
	if (FAILED(result))
		Console.error("Error Disable Blend State");

	return hr;
}

HRESULT CoreEngine::CreatePlayerDirect3DContext(HWND wndHandle)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	UINT deviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif		
	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
		pBackBuffer->Release();

		// set the render target as the back buffer
		gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, NULL);
	}
	return hr;
}

bool CoreEngine::CreateDepthStencilBuffer()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&m_depthStencilBuffer, sizeof(m_depthStencilBuffer));

	Input.GetEngineWindowResolution();

	RECT rect;

	int width = 0, height = 0;

	if (GetWindowRect(wnd, &rect))
	{
		width = Input.GetEngineWindowResolution().x;
		height = Input.GetEngineWindowResolution().y;
		/*width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		width /= 2;
		height /= 2;
		width -= 8;
		height -= 20;*/
		/*width -= 16;
		height -= 39;*/
	}

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	HRESULT result = gDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	return !FAILED(result);
}

bool CoreEngine::CreateDepthStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	HRESULT result = gDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

	return !(FAILED(result));
}

bool CoreEngine::CreateDepthStencilState(void * ds, bool enabled, bool writeEnabled)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// Depth test parameters
	desc.DepthEnable = enabled ? 1 : 0; // true or false
	desc.DepthWriteMask = writeEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	desc.StencilEnable = true;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// Stencil operations if pixel is front-facing
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create a depth stencil state with depth enabled
	ID3D11DepthStencilState* depthStencilStateTyped = (ID3D11DepthStencilState*)ds;
	auto result = gDevice->CreateDepthStencilState(&desc, &depthStencilStateTyped);

	return !FAILED(result);
}

bool CoreEngine::CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState ** rasterizer)
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = fillMode;
	desc.CullMode = cullMode;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;

	HRESULT result = gDevice->CreateRasterizerState(&desc, rasterizer);

	if (FAILED(result))
	{
		Console.error("Couldn't create rasterizer---function");
		return false;
	}

	return true;
}
