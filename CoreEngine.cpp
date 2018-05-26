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
#include <future>
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
	gamemanager.~GameManager();
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
		CreateDirect3DContext(wndHandle);

		SetViewport(); //3. S�tt viewport


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
		AssetManager.addShaderProgram("FragmentOpacity.hlsl", SHADER_TYPE::PIXEL_SHADER);


		//shaderProgram.CreateShaderData(gDeviceContext, gDevice, descTest, "Vertex.hlsl", "", "", "", "Fragment.hlsl", "");


		addMaterials();

		//----------------
		GameManager gameManager = GameManager(gDevice, gDeviceContext);
		PathCreator = cPathCreator(300, 300);
		if (!PLAYER_BUILD)
			createTerrain();

		//Meshes
		AssetManager.addMeshFromBinary("Assets/PIRATE.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/BarracksTest1.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.addMeshFromBinary("Assets/QuarryTwo1_Mesh.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Spruce_Tree2.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Test2ResourceSilo.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Barracks/CryptBarracksFinale1.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/ResourceCenter/FinalSilo1OBJ.obj", AssetManager.getShaderProgram("Vertex.hlsl")); 
		AssetManager.addMeshFromBinary("Assets/COLLECTOR_Mesh.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Stairs.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.addMesh("Assets/FlowersAndBushes4.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Fern.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/LionPillar.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		//AssetManager.AddMesh("Assets/Worker/Worker.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/RuinedPillar.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Pillar.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.AddMesh("Assets/Brazier.obj", AssetManager.getShaderProgram("Vertex.hlsl"));

		AssetManager.addMeshFromBinary("Assets/Projektil_Mesh.bin", AssetManager.getShaderProgram("Vertex.hlsl"));

		AssetManager.AddMesh("Assets/Hero1.obj", AssetManager.getShaderProgram("Vertex.hlsl"));
		AssetManager.addMeshFromBinary("Assets/Soldier/Soldier1.bin", AssetManager.getShaderProgram("Vertex.hlsl"));


		//PathCreator.createNodes();

		// Create a Main Camera
		Camera* cam = nullptr;
		camera = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 75, 0, 0)); //(DirectX::XMVectorSet(0, 25, 0, 0));
		camera->name = "Main Camera";
		cam = new Camera(HEIGHT, WIDTH, 30, 0.01f, 1000.0f);
		camera->transform.setRotation(DirectX::XMVectorSet(0, 0, 70, 0)); //(DirectX::XMVectorSet(0, 0, 70, 0));
		camera->addComponent(cam);
		PlayerScript* playerscript = new PlayerScript(camera);
		camera->addComponent(playerscript);
		Light* directionalLight = new Light(LIGHT_TYPES::Directional, SHADOW_TYPE::SoftShadows, TEXTURE_RESOLUTIONS::R1024x1024, DirectX::XMVectorSet(1, 1, 1, 1), 1, 1);
		camera->addComponent(directionalLight);
		//PlayerScript *playerscript = new PlayerScript();
		//camera->addComponent(playerscript);

		
		//Barrack
		AssetManager.addMeshFromBinary("Assets/COLLECTOR_Mesh.bin", AssetManager.getShaderProgram("Vertex.hlsl"));
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




		//KOMMENTERA IN FÖR ATT VISA ANIMATION

		//
	//	GameObject* animatedGO = gScene.createEmptyGameObject(DirectX::XMVectorSet(150, 30, 130, 0));
	//	animatedGO->name = "Animator";
	//	AssetManager.addAnimatedMeshFromBinary("Assets/pCube1_ANIMATION_Mesh.bin", AssetManager.getShaderProgram("VertexAnimation.hlsl"));
	//	Mesh* animMesh = AssetManager.getMesh("pCube1_ANIMATION_Mesh");
	//	MeshFilter* animMeshFilter = new MeshFilter(animMesh);
	//	animatedGO->addComponent(animMeshFilter);
	//
	//	AssetManager.addSkeletonFromBinary("Assets/joint1_Skeleton.bin");
	//	Animator* animator = new Animator(assetManager.getSkeleton("joint1_Skeleton"));
	//	animatedGO->addComponent(animator);
	//
	//	AssetManager.addAnimationClipFromBinary(assetManager.getSkeleton("joint1_Skeleton"), "Assets/ANIMATION_ANIMATION.bin");
	//	animator->addAnimationClip(AssetManager.getAnimationclip(assetManager.getSkeleton("joint1_Skeleton"), "ANIMATION_ANIMATION"));
	//	animator->Play(0, true);
	//
	//	animatedGO->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));
	
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

		while (WM_QUIT != msg.message && IS_RUNNING)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				 //FULLSCREEN
				{
					BOOL bFullscreen;
					gSwapChain->GetFullscreenState(&bFullscreen, nullptr);
					// If not full screen, enable fullscreen again.
					if (!bFullscreen)
						gSwapChain->SetFullscreenState(TRUE, NULL);
				}

				inputHandler.updateInput();
				Time.tick();
				gameManager.update();
				OnResize();

				gScene.destroyGameObjects();
				gScene.frustumCull(camera);
				objectsToRender = gScene.getFrustumCulledResult();

				gScene.update();
				if (editor != nullptr)
					editor->Update();
				if (player != nullptr)
					player->Update();


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

		gameManager.~GameManager();
		delete renderManager;
		DestroyWindow(wndHandle);
	}
	return msg;
}


void CoreEngine::addMaterials()
{
	//Terrain Texture.
	assetManager.addTexture("Assets/Grass-Diffuse.png"); //3
	assetManager.addTexture("Assets/Grass-Normal.png"); //4
	assetManager.addTexture("Assets/Grass-RoughMetalAo.png"); //5

	assetManager.addTexture("Assets/Sand_BaseColor.png"); //3
	assetManager.addTexture("Assets/Sand_Normal.png"); //4
	assetManager.addTexture("Assets/Sand_OcclusionRoughnessMetallic.png"); //5

	assetManager.addTexture("Assets/SandStone_BaseColor.png"); //6
	assetManager.addTexture("Assets/SandStone_Normal.png"); //7
	assetManager.addTexture("Assets/SandStone_OcclusionRoughnessMetallic.png"); //8

	assetManager.addTexture("Assets/GrassBricks-Diffuse.png"); //9
	assetManager.addTexture("Assets/GrassBricks-Normal.png"); //10
	assetManager.addTexture("Assets/GrassBricks-RoughMetalAo.png"); //11

	assetManager.addTexture("Assets/Lava_Albedo.png"); //20
	assetManager.addTexture("Assets/Lava_Normal.png"); //21
	assetManager.addTexture("Assets/Lava_OcclusionRoughnessMetallic.png"); //22

	assetManager.addTexture("Assets/FinaleIDMAP8.png");

	//Terrain
	int matXTile = 20;
	int matYTile = 20;

	//Terrain Material 1
	assetManager.AddMaterial("TerrainMaterial1", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("TerrainMaterial1")->setIsTerrain(true);
	assetManager.getMaterial("TerrainMaterial1")->setXTile(matXTile);
	assetManager.getMaterial("TerrainMaterial1")->setYTile(matYTile);
	assetManager.getMaterial("TerrainMaterial1")->setAlbedo(assetManager.getTexture("Grass-Diffuse")->getTexture());
	assetManager.getMaterial("TerrainMaterial1")->setNormal(assetManager.getTexture("Grass-Normal")->getTexture());
	assetManager.getMaterial("TerrainMaterial1")->setAORoughMet(assetManager.getTexture("Grass-RoughMetalAo")->getTexture());

	assetManager.getMaterial("TerrainMaterial1")->setTerrainMaterials(

	assetManager.getTexture("Sand_BaseColor")->getTexture(),
	assetManager.getTexture("Sand_Normal")->getTexture(),
	assetManager.getTexture("Sand_OcclusionRoughnessMetallic")->getTexture(),

	assetManager.getTexture("SandStone_BaseColor")->getTexture(),
	assetManager.getTexture("SandStone_Normal")->getTexture(),
	assetManager.getTexture("GrassBricks-RoughMetalAo")->getTexture(),

	assetManager.getTexture("GrassBricks-Diffuse")->getTexture(),
	assetManager.getTexture("GrassBricks-Normal")->getTexture(),
	assetManager.getTexture("GrassBricks-RoughMetalAo")->getTexture(),

	assetManager.getTexture("Lava_Albedo")->getTexture(),
	assetManager.getTexture("Lava_Normal")->getTexture(),
	assetManager.getTexture("Lava_OcclusionRoughnessMetallic")->getTexture(),

	assetManager.getTexture("FinaleIDMAP8")->getTexture()); //USE ID_PART 1



																//Unit & Tree textures
	AssetManager.addTexture("Assets/Soldier/STSP_ShadowTeam_BaseColor.png");
	AssetManager.addTexture("Assets/Soldier/STSP_ShadowTeam_Normal.png");
	AssetManager.addTexture("Assets/Soldier/Soldier1_Id.png");
	AssetManager.addTexture("Assets/Soldier/STSP_ShadowTeam_OcclusionRoughnessMetallic.png");
	AssetManager.addTexture("Assets/troll_made_with_unity.png");

	CreateUITextures();

	//barracksTex
	assetManager.AddTexture("Assets/Barracks/CryptBarracksFinale1_Barracks_BaseColor.png");
	assetManager.AddTexture("Assets/Barracks/CryptBarracksFinale1_Barracks_Emissive.png");
	assetManager.AddTexture("Assets/Barracks/CryptBarracksFinale1_Barracks_Normal.png");
	assetManager.AddTexture("Assets/Barracks/CryptBarracksFinale1_Barracks_OcclusionRoughnessMetallic.png");

	//ResourseCenterTex
	assetManager.AddTexture("Assets/ResourceCenter/FinalSilo1OBJ_ResourceSilo_BaseColor.png");
	assetManager.AddTexture("Assets/ResourceCenter/FinalSilo1OBJ_ResourceSilo_Normal.png");
	assetManager.AddTexture("Assets/ResourceCenter/FinalSilo1OBJ_ResourceSilo_OcclusionRoughnessMetallic.png");
	assetManager.AddTexture("Assets/ResourceCenter/FinalSilo1OBJ_ResourceSilo_Emissive.png");

	//Goldmine
	assetManager.AddTexture("Assets/Goldmine/GoldMineResource_BaseColor.png");
	assetManager.AddTexture("Assets/Goldmine/GoldMineResource_Normal.png");
	assetManager.AddTexture("Assets/Goldmine/GoldMineResource_OcclusionRoughnessMetallic.png");
	

	AssetManager.addTexture("Assets/Spruce_Tree1_initialShadingGroup_BaseColor.png");
	AssetManager.addTexture("Assets/Spruce_Tree1_initialShadingGroup_Normal.png");
	AssetManager.addTexture("Assets/Spruce_Tree1_initialShadingGroup_OcclusionRoughnessMetallic.png");


	AssetManager.addTexture("Assets/Stairs_stairs_BaseColor.png");
	AssetManager.addTexture("Assets/Stairs_stairs_Normal.png");
	AssetManager.addTexture("Assets/Stairs_stairs_OcclusionRoughnessMetallic.png");

	AssetManager.addTexture("Assets/Fern_Fern_BaseColor.png");
	AssetManager.addTexture("Assets/Fern_Fern_Normal.png");
	AssetManager.addTexture("Assets/Fern_Fern_OcclusionRoughnessMetallic.png");

	AssetManager.addTexture("Assets/Brazier_Brazier_BaseColor.png");
	AssetManager.addTexture("Assets/Brazier_Brazier_Normal.png");
	AssetManager.addTexture("Assets/Brazier_Brazier_OcclusionRoughnessMetallic.png");

	AssetManager.addTexture("Assets/RuinedPillar_BrokenPillar_BaseColor.png");
	AssetManager.addTexture("Assets/RuinedPillar_BrokenPillar_Normal.png");
	AssetManager.addTexture("Assets/RuinedPillar_BrokenPillar_OcclusionRoughnessMetallic.png");

	AssetManager.addTexture("Assets/Pillar_Pillar_BaseColor.png");
	AssetManager.addTexture("Assets/Pillar_Pillar_Normal.png");
	AssetManager.addTexture("Assets/Pillar_Pillar_OcclusionRoughnessMetallic.png");

	assetManager.addTexture("Assets/FlowersAndBushes1_FlowerGrowth_BaseColor.png");
	assetManager.addTexture("Assets/FlowersAndBushes1_FlowerGrowth_Normal.png");
	assetManager.addTexture("Assets/FlowersAndBushes1_FlowerGrowth_OcclusionRoughnessMetallic.png");

	AssetManager.addTexture("Assets/LionPillar_LionPillar_BaseColor.png");
	AssetManager.addTexture("Assets/LionPillar_LionPillar_ormal.png");
	AssetManager.addTexture("Assets/LionPillar_LionPillar_OcclusionRoughnessMetallic.png");

	AssetManager.addTexture("Assets/Hero1_Hero_BaseColor.png");
	AssetManager.addTexture("Assets/Hero1_Hero_Normal.png");
	AssetManager.addTexture("Assets/Hero1_Hero_OcclusionRoughnessMetallic.png");
	AssetManager.addTexture("Assets/Hero1_Hero_Emissive.png");


	//GoldmineMat
	assetManager.AddMaterial("GoldMineMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("GoldMineMaterial")->setAlbedo(assetManager.getTexture("GoldMineResource_BaseColor")->getTexture());
	assetManager.getMaterial("GoldMineMaterial")->setNormal(assetManager.getTexture("GoldMineResource_Normal")->getTexture());
	assetManager.getMaterial("GoldMineMaterial")->setAORoughMet(assetManager.getTexture("GoldMineResource_OcclusionRoughnessMetallic")->getTexture());


	//BarracksMat
	assetManager.AddMaterial("BarracksMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("BarracksMaterial")->setAlbedo(assetManager.getTexture("CryptBarracksFinale1_Barracks_BaseColor")->getTexture());
	assetManager.getMaterial("BarracksMaterial")->setNormal(assetManager.getTexture("CryptBarracksFinale1_Barracks_Normal")->getTexture());
	assetManager.getMaterial("BarracksMaterial")->setAORoughMet(assetManager.getTexture("CryptBarracksFinale1_Barracks_OcclusionRoughnessMetallic")->getTexture());
	assetManager.getMaterial("BarracksMaterial")->setTeamIdMap(assetManager.getTexture("CryptBarracksFinale1_Barracks_Emissive")->getTexture());

	//ResourceMaterial
	assetManager.AddMaterial("ResourceMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("ResourceMaterial")->setAlbedo(assetManager.getTexture("FinalSilo1OBJ_ResourceSilo_BaseColor")->getTexture());
	assetManager.getMaterial("ResourceMaterial")->setNormal(assetManager.getTexture("FinalSilo1OBJ_ResourceSilo_Normal")->getTexture());
	assetManager.getMaterial("ResourceMaterial")->setAORoughMet(assetManager.getTexture("FinalSilo1OBJ_ResourceSilo_OcclusionRoughnessMetallic")->getTexture());
	assetManager.getMaterial("ResourceMaterial")->setTeamIdMap(assetManager.getTexture("FinalSilo1OBJ_ResourceSilo_Emissive")->getTexture());

	//RuinedPillarMaterial
	assetManager.AddMaterial("RuinedPillarMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("RuinedPillarMaterial")->setAlbedo(assetManager.getTexture("RuinedPillar_BrokenPillar_BaseColor")->getTexture());
	assetManager.getMaterial("RuinedPillarMaterial")->setNormal(assetManager.getTexture("RuinedPillar_BrokenPillar_Normal")->getTexture());
	assetManager.getMaterial("RuinedPillarMaterial")->setAORoughMet(assetManager.getTexture("RuinedPillar_BrokenPillar_RoughMetalAo")->getTexture());

	//PillarMaterial
	assetManager.AddMaterial("PillarMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("PillarMaterial")->setAlbedo(assetManager.getTexture("Pillar_Pillar_BaseColor")->getTexture());
	assetManager.getMaterial("PillarMaterial")->setNormal(assetManager.getTexture("Pillar_Pillar_Normal")->getTexture());
	assetManager.getMaterial("PillarMaterial")->setAORoughMet(assetManager.getTexture("Pillar_Pillar_RoughMetalAo")->getTexture());

	//BrazierMaterial
	assetManager.AddMaterial("BrazierMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("BrazierMaterial")->setAlbedo(assetManager.getTexture("Brazier_Brazier_BaseColor")->getTexture());
	assetManager.getMaterial("BrazierMaterial")->setNormal(assetManager.getTexture("Brazier_Brazier_Normal")->getTexture());
	assetManager.getMaterial("BrazierMaterial")->setAORoughMet(assetManager.getTexture("Brazier_Brazier_RoughMetalAo")->getTexture());

	//Unit Material

	//Soldier material
	assetManager.AddMaterial("SoldierMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("SoldierMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("SoldierMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("SoldierMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());
	assetManager.getMaterial("SoldierMaterial")->setTeamIdMap(assetManager.getTexture("Soldier1_Id")->getTexture());

	//Hero material
	assetManager.AddMaterial("HeroMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("HeroMaterial")->setAlbedo(assetManager.getTexture("Hero1_Hero_BaseColor")->getTexture());
	assetManager.getMaterial("HeroMaterial")->setNormal(assetManager.getTexture("Hero1_Hero_Normal")->getTexture());
	assetManager.getMaterial("HeroMaterial")->setAORoughMet(assetManager.getTexture("Hero1_Hero_OcclusionRoughnessMetallic")->getTexture());
	assetManager.getMaterial("HeroMaterial")->setTeamIdMap(assetManager.getTexture("Hero1_Hero_Emissive")->getTexture());

	//Worker
	AssetManager.addTexture("Assets/Worker/WorkerShipFinale_WorkerShip_BaseColor.png");
	AssetManager.addTexture("Assets/Worker/WorkerShipFinale_WorkerShip_Normal.png");
	AssetManager.addTexture("Assets/Worker/WorkerShipFinale_WorkerShip_OcclusionRoughnessMetallic.png");
	AssetManager.addTexture("Assets/Worker/WorkerShipFinale_WorkerShip_Emissive.png");

	assetManager.AddMaterial("WorkerMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("WorkerMaterial")->setAlbedo(assetManager.getTexture("WorkerShipFinale_WorkerShip_BaseColor")->getTexture());
	assetManager.getMaterial("WorkerMaterial")->setNormal(assetManager.getTexture("WorkerShipFinale_WorkerShip_Normal")->getTexture());
	assetManager.getMaterial("WorkerMaterial")->setAORoughMet(assetManager.getTexture("WorkerShipFinale_WorkerShip_OcclusionRoughnessMetallic")->getTexture());
	assetManager.getMaterial("WorkerMaterial")->setTeamIdMap(assetManager.getTexture("WorkerShipFinale_WorkerShip_Emissive")->getTexture());

	
	//Bank material
	assetManager.AddMaterial("BankMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("BankMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("BankMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("BankMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());

	//Goldmine material
	/*assetManager.AddMaterial("GoldmineMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("GoldmineMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("GoldmineMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("GoldmineMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());
*/
	//Barrack material
	assetManager.AddMaterial("BarrackMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("BarrackMaterial")->setAlbedo(assetManager.getTexture("STSP_ShadowTeam_BaseColor")->getTexture());
	assetManager.getMaterial("BarrackMaterial")->setNormal(assetManager.getTexture("STSP_ShadowTeam_Normal")->getTexture());
	assetManager.getMaterial("BarrackMaterial")->setAORoughMet(assetManager.getTexture("STSP_ShadowTeam_OcclusionRoughnessMetallic")->getTexture());


	//Tree Material
	assetManager.AddMaterial("TreeMaterial", assetManager.getShaderProgram("FragmentOpacity.hlsl"));
	assetManager.getMaterial("TreeMaterial")->setAlpha(true);
	assetManager.getMaterial("TreeMaterial")->setAlbedo(assetManager.getTexture("Spruce_Tree1_initialShadingGroup_BaseColor")->getTexture());
	assetManager.getMaterial("TreeMaterial")->setNormal(assetManager.getTexture("Spruce_Tree1_initialShadingGroup_Normal")->getTexture());
	assetManager.getMaterial("TreeMaterial")->setAORoughMet(assetManager.getTexture("Spruce_Tree1_initialShadingGroup_OcclusionRoughnessMetallic")->getTexture());

	//Stairs Material
	assetManager.AddMaterial("StairsMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("StairsMaterial")->setAlbedo(assetManager.getTexture("Stairs_stairs_BaseColor")->getTexture());
	assetManager.getMaterial("StairsMaterial")->setNormal(assetManager.getTexture("Stairs_stairs_Normal")->getTexture());
	assetManager.getMaterial("StairsMaterial")->setAORoughMet(assetManager.getTexture("Stairs_stairs_OcclusionRoughnessMetallic")->getTexture());

	//Fern Material
	assetManager.AddMaterial("FernMaterial", assetManager.getShaderProgram("FragmentOpacity.hlsl"));
	assetManager.getMaterial("FernMaterial")->setAlpha(true);
	assetManager.getMaterial("FernMaterial")->setAlbedo(assetManager.getTexture("Fern_Fern_BaseColor")->getTexture());
	assetManager.getMaterial("FernMaterial")->setNormal(assetManager.getTexture("Fern_Fern_Normal")->getTexture());
	assetManager.getMaterial("FernMaterial")->setAORoughMet(assetManager.getTexture("Fern_Fern_OcclusionRoughnessMetallic")->getTexture());

	//Flower Material
	assetManager.AddMaterial("FlowersAndBushesMaterial", assetManager.getShaderProgram("FragmentOpacity.hlsl"));
	assetManager.getMaterial("FlowersAndBushesMaterial")->setAlpha(true);
	assetManager.getMaterial("FlowersAndBushesMaterial")->setAlbedo(assetManager.getTexture("FlowersAndBushes1_FlowerGrowth_BaseColor")->getTexture());
	assetManager.getMaterial("FlowersAndBushesMaterial")->setNormal(assetManager.getTexture("FlowersAndBushes1_FlowerGrowth_Normal")->getTexture());
	assetManager.getMaterial("FlowersAndBushesMaterial")->setAORoughMet(assetManager.getTexture("FlowersAndBushes1_FlowerGrowth_OcclusionRoughnessMetallic")->getTexture());

	//Lion Pillar Material
	assetManager.AddMaterial("LionPillarMaterial", assetManager.getShaderProgram("Fragment.hlsl"));
	assetManager.getMaterial("LionPillarMaterial")->setAlbedo(assetManager.getTexture("LionPillar_LionPillar_BaseColor")->getTexture());
	assetManager.getMaterial("LionPillarMaterial")->setNormal(assetManager.getTexture("LionPillar_LionPillar_Normal")->getTexture());
	assetManager.getMaterial("LionPillarMaterial")->setAORoughMet(assetManager.getTexture("LionPillar_LionPillar_OcclusionRoughnessMetallic")->getTexture());

}

#include <experimental\filesystem>

void CoreEngine::CreateUITextures()
{
	namespace fs = std::experimental::filesystem;

	std::string path = "Assets/UI";
	for (auto & p : fs::directory_iterator(path))
	{
		std::string finalPath = p.path().string();

		char remove = '\\';
		for (string::iterator it = finalPath.begin(); it != finalPath.end();)
		{
			if ((*it) == remove)
				it = finalPath.erase(it);
			else
				it++;
		}

		finalPath.insert(6, "/");
		finalPath.insert(9, "/");

		AssetManager.addTexture(finalPath);

		//AssetManager.addTexture(finalPath);
	}
}

int CoreEngine::createTerrain()
{
	TerrainGenerator* Terrain = new TerrainGenerator(300, 300, "Assets/FinaleHeightMapTotal.bmp");


	GameObject* terrain1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain1->name = "Terrain1";
	terrain1->tag = 0;
	terrain1->raycastOption = RayCastOptions::DETAILED;
	Mesh* m1 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_1", Terrain->getPatchTriangleArr(0, 0, 100, 100), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain = new MeshFilter(m1);
	terrain1->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain1->addComponent(meshFilterTerrain);

	GameObject* terrain2 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain2->name = "Terrain2";
	terrain2->tag = 0;
	terrain2->raycastOption = RayCastOptions::DETAILED;
	Mesh* m2 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_2", Terrain->getPatchTriangleArr(100, 0, 200, 100), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain2 = new MeshFilter(m2);
	terrain2->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain2->addComponent(meshFilterTerrain2);

	GameObject* terrain3 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain3->name = "Terrain3";
	terrain3->tag = 0;
	terrain3->raycastOption = RayCastOptions::DETAILED;
	Mesh* m3 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_3", Terrain->getPatchTriangleArr(200, 0, 300, 100), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain3 = new MeshFilter(m3);
	terrain3->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain3->addComponent(meshFilterTerrain3);

	GameObject* terrain4 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain4->name = "Terrain4";
	terrain4->tag = 0;
	terrain4->raycastOption = RayCastOptions::DETAILED;
	Mesh* m4 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_4", Terrain->getPatchTriangleArr(0, 100, 100, 200), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain4 = new MeshFilter(m4);
	terrain4->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain4->addComponent(meshFilterTerrain4);

	GameObject* terrain5 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain5->name = "Terrain5";
	terrain5->tag = 0;
	terrain5->raycastOption = RayCastOptions::DETAILED;
	Mesh* m5 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_5", Terrain->getPatchTriangleArr(100, 100, 200, 200), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain5 = new MeshFilter(m5);
	terrain5->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain5->addComponent(meshFilterTerrain5);

	GameObject* terrain6 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain6->name = "Terrain6";
	terrain6->tag = 0;
	terrain6->raycastOption = RayCastOptions::DETAILED;
	Mesh* m6 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_6", Terrain->getPatchTriangleArr(200, 100, 300, 200), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain6 = new MeshFilter(m6);
	terrain6->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain6->addComponent(meshFilterTerrain6);

	GameObject* terrain7 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain7->name = "Terrain7";
	terrain7->tag = 0;
	terrain7->raycastOption = RayCastOptions::DETAILED;
	Mesh* m7 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_7", Terrain->getPatchTriangleArr(0, 200, 100, 300), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain7 = new MeshFilter(m7);
	terrain7->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain7->addComponent(meshFilterTerrain7);

	GameObject* terrain8 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain8->name = "Terrain8";
	terrain8->tag = 0;
	terrain8->raycastOption = RayCastOptions::DETAILED;
	Mesh* m8 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_8", Terrain->getPatchTriangleArr(100, 200, 200, 300), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain8 = new MeshFilter(m8);
	terrain8->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain8->addComponent(meshFilterTerrain8);

	GameObject* terrain9 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
	terrain9->name = "Terrain9";
	terrain9->tag = 0;
	terrain9->raycastOption = RayCastOptions::DETAILED;
	Mesh* m9 = AssetManager.AddMesh(Terrain->getPatchVertCount(), "Terrain_9", Terrain->getPatchTriangleArr(200, 200, 300, 300), AssetManager.getShaderProgram("Vertex.hlsl"));
	MeshFilter* meshFilterTerrain9 = new MeshFilter(m9);
	terrain9->addComponent(new MaterialFilter(AssetManager.getMaterial("TerrainMaterial1")));
	terrain9->addComponent(meshFilterTerrain9);

	//PathCreator.createNodes(terrainGenerator1->getRealVertArr());


	PathCreator.addTerrain(Terrain->getRealVertArr(), 0, 0);
	//PathCreator1->addTerrain(terrainGenerator3->getRealVertArr(), 0, 198);
	//PathCreator1->addTerrain(terrainGenerator4->getRealVertArr(), 99, 0);
	//PathCreator1->addTerrain(terrainGenerator5->getRealVertArr(), 99, 99);
	//PathCreator1->addTerrain(terrainGenerator6->getRealVertArr(), 99, 198);
	//PathCreator1->addTerrain(terrainGenerator8->getRealVertArr(), 198, 99);
	//PathCreator1->addTerrain(terrainGenerator9->getRealVertArr(), 198, 198);
	//PathCreator1->addTerrain(terrainGenerator7->getRealVertArr(), 198, 0);

	PathCreator.trumpTheBorders();
	delete Terrain;
	//PathCreator.createNodes();
	return 1;
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
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Create the blend state using the description.
	result = gDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendState);
	if (FAILED(result))
		Console.error("Error Enable Blend State");

	UINT sampleMask = 0xffffffff;
	gDeviceContext->OMSetBlendState(m_alphaEnableBlendState, 0, sampleMask);
	//// Modify the description to create an alpha disabled blend state description.
	//blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	//// Create the blend state using the description.
	//result = gDevice->CreateBlendState(&blendStateDescription, &m_alphaDisabledBlendState);
	//if (FAILED(result))
	//	Console.error("Error Disable Blend State");

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