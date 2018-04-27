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

#include "Debug.h"

#include <crtdbg.h>
#include "PathCreator.h"
#pragma comment(lib, "dxgi.lib")

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 

bool CoreEngine::hasResized = false;

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
	AssetManager.~cAssetManager();
	gScene.~Scene();
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


		D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{
				"POSITION",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				0,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},
			{
				"UV",
				0,				// same slot as previous (same vertexBuffer)
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				12,							// offset of FIRST element (after POSITION)
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"TANGENT",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				20,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},{
				"BITANGENT",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				32,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},{
				"NORMAL",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				44,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},
		};
		std::vector<D3D11_INPUT_ELEMENT_DESC> descTest;
		descTest.push_back(inputDesc[0]);
		descTest.push_back(inputDesc[1]);
		descTest.push_back(inputDesc[2]);
		descTest.push_back(inputDesc[3]);
		descTest.push_back(inputDesc[4]);

		Time.start();

		renderManager = new RenderManager(gDevice, gDeviceContext, gBackbufferRTV, gSwapChain, m_depthStencilView);
		renderManager->CreateRenderTarget(WIDTH, HEIGHT);

		InputHandler inputHandler = InputHandler(wndHandle); // this has memory leak

		AssetManager.Start(gDevice, gDeviceContext);

		//assetManager = cAssetManager(gDevice, gDeviceContext); // this has memory leak
		AssetManager.addShaderProgram(INPUT_ELEMENT_DESCRIPTION::INPUT_ELEMENT_POS3UV2T3B3N3, "Vertex.hlsl", "", "", "", "Fragment.hlsl", "");

		//shaderProgram.CreateShaderData(gDeviceContext, gDevice, descTest, "Vertex.hlsl", "", "", "", "Fragment.hlsl", "");



		AssetManager.addTexture("Assets/STSP_ShadowTeam_BaseColor.png");
		AssetManager.addTexture("Assets/STSP_ShadowTeam_Normal.png");
		AssetManager.addTexture("Assets/STSP_ShadowTeam_OcclusionRoughnessMetallic.png");
		
		//Terrain Texture.
		assetManager.addTexture("Assets/rutTextur.png"); //3
		assetManager.addTexture("Assets/rutNormal.png"); //4
		assetManager.addTexture("Assets/rutAoMetalRough.png"); //5

		assetManager.addTexture("Assets/stenTextur.png"); //6
		assetManager.addTexture("Assets/stenNormal.png"); //7
		assetManager.addTexture("Assets/stenAoMetalRough.png"); //8

		assetManager.addTexture("Assets/tygTextur.png"); //9
		assetManager.addTexture("Assets/tygNormal.png"); //10
		assetManager.addTexture("Assets/tygAoMetalRough.png"); //11

		assetManager.addTexture("Assets/ID_MAP2.png"); //12
		assetManager.addTexture("Assets/vitTextur.png"); //13
		assetManager.addTexture("Assets/vitNormal.png"); //14
		assetManager.addTexture("Assets/vitAoMetalRough.png"); //15

		assetManager.addTexture("Assets/ID_MAP2part1.png"); // 16 // 1 // ALL THE PARTS SHOULD BE ROTATED CLOCKEWISE 90 DEGREES TO MATCH WITH BMP.
		assetManager.addTexture("Assets/ID_MAP2part2.png"); // 17 // 2
		assetManager.addTexture("Assets/ID_MAP2part3.png"); // 18 // 3
		assetManager.addTexture("Assets/ID_MAP2part4.png"); //19 // 4


		assetManager.addMaterial(assetManager.getShaderProgram(0));

		assetManager.getMaterial(0)->setAlbedo(assetManager.getTexture(0)->getTexture());
		assetManager.getMaterial(0)->setNormal(assetManager.getTexture(1)->getTexture());
		assetManager.getMaterial(0)->setAORoughMet(assetManager.getTexture(2)->getTexture());
		assetManager.getMaterial(0)->setIsTerrain(false);

		assetManager.addMaterial(assetManager.getShaderProgram(0));
		assetManager.getMaterial(1)->setIsTerrain(true);
		assetManager.getMaterial(1)->setTerrainMaterials(assetManager.getTexture(3)->getTexture(), assetManager.getTexture(4)->getTexture(), assetManager.getTexture(5)->getTexture(),
		assetManager.getTexture(6)->getTexture(), assetManager.getTexture(7)->getTexture(), assetManager.getTexture(8)->getTexture(), assetManager.getTexture(9)->getTexture(),
		assetManager.getTexture(10)->getTexture(), assetManager.getTexture(11)->getTexture(), assetManager.getTexture(18)->getTexture()); //USE ID_PART 3

		assetManager.addMaterial(assetManager.getShaderProgram(0));
		assetManager.getMaterial(2)->setIsTerrain(true);
		assetManager.getMaterial(2)->setTerrainMaterials(assetManager.getTexture(3)->getTexture(), assetManager.getTexture(4)->getTexture(), assetManager.getTexture(5)->getTexture(),
			assetManager.getTexture(6)->getTexture(), assetManager.getTexture(7)->getTexture(), assetManager.getTexture(8)->getTexture(), assetManager.getTexture(9)->getTexture(),
			assetManager.getTexture(10)->getTexture(), assetManager.getTexture(11)->getTexture(), assetManager.getTexture(16)->getTexture()); //USE ID_PART 1

		assetManager.addMaterial(assetManager.getShaderProgram(0));
		assetManager.getMaterial(3)->setIsTerrain(true);
		assetManager.getMaterial(3)->setTerrainMaterials(assetManager.getTexture(3)->getTexture(), assetManager.getTexture(4)->getTexture(), assetManager.getTexture(5)->getTexture(),
			assetManager.getTexture(6)->getTexture(), assetManager.getTexture(7)->getTexture(), assetManager.getTexture(8)->getTexture(), assetManager.getTexture(9)->getTexture(),
			assetManager.getTexture(10)->getTexture(), assetManager.getTexture(11)->getTexture(), assetManager.getTexture(19)->getTexture()); //USE ID_PART 4

		assetManager.addMaterial(assetManager.getShaderProgram(0));
		assetManager.getMaterial(4)->setIsTerrain(true);
		assetManager.getMaterial(4)->setTerrainMaterials(assetManager.getTexture(3)->getTexture(), assetManager.getTexture(4)->getTexture(), assetManager.getTexture(5)->getTexture(),
			assetManager.getTexture(6)->getTexture(), assetManager.getTexture(7)->getTexture(), assetManager.getTexture(8)->getTexture(), assetManager.getTexture(9)->getTexture(),
			assetManager.getTexture(10)->getTexture(), assetManager.getTexture(11)->getTexture(), assetManager.getTexture(17)->getTexture()); //USE ID_PART 2

		//----------------

		GameObject* terrain1 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 0, 0));
		terrain1->name = "Terrain1";
		terrain1->tag = 0;
		terrain1->detailedRaycast = true;
		TerrainGenerator* terrainGenerator1 = new TerrainGenerator(100, 100, "Assets/BmpMap3Part3.bmp");
		AssetManager.addMesh(terrainGenerator1->vertCount, &terrainGenerator1->TriangleArr);
		MeshFilter* meshFilterTerrain = new MeshFilter(AssetManager.getMesh(0));
		terrain1->addComponent(AssetManager.getMaterial(1));
		terrain1->addComponent(meshFilterTerrain);

		GameObject* terrain2 = gScene.createEmptyGameObject(DirectX::XMVectorSet(99, 0, 0, 0));
		terrain2->name = "Terrain2";
		terrain2->tag = 0;
		terrain2->detailedRaycast = true;
		TerrainGenerator* terrainGenerator2 = new TerrainGenerator(100, 100, "Assets/BmpMap3Part1.bmp");
		AssetManager.addMesh(terrainGenerator2->vertCount, &terrainGenerator2->TriangleArr);
		MeshFilter* meshFilterTerrain2 = new MeshFilter(AssetManager.getMesh(1));
		terrain2->addComponent(AssetManager.getMaterial(2));
		terrain2->addComponent(meshFilterTerrain2);

		GameObject* terrain3 = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 0, 99, 0));
		terrain3->name = "Terrain3";
		terrain3->tag = 0;
		terrain3->detailedRaycast = true;
		TerrainGenerator* terrainGenerator3 = new TerrainGenerator(100, 100, "Assets/BmpMap3Part4.bmp");
		AssetManager.addMesh(terrainGenerator3->vertCount, &terrainGenerator3->TriangleArr);
		MeshFilter* meshFilterTerrain3 = new MeshFilter(AssetManager.getMesh(2));
		terrain3->addComponent(AssetManager.getMaterial(3));
		terrain3->addComponent(meshFilterTerrain3);

		GameObject* terrain4 = gScene.createEmptyGameObject(DirectX::XMVectorSet(99, 0, 99, 0));
		terrain4->name = "Terrain4";
		terrain4->tag = 0;
		terrain4->detailedRaycast = true;
		TerrainGenerator* terrainGenerator4 = new TerrainGenerator(100, 100, "Assets/BmpMap3Part2.bmp");
		AssetManager.addMesh(terrainGenerator4->vertCount, &terrainGenerator4->TriangleArr);
		MeshFilter* meshFilterTerrain4 = new MeshFilter(AssetManager.getMesh(3));
		terrain4->addComponent(AssetManager.getMaterial(4));
		terrain4->addComponent(meshFilterTerrain4);

		//PathCreator.createNodes(terrainGenerator1->getRealVertArr());
		cPathCreator* PathCreator1 = new cPathCreator(200, 200);

		PathCreator1->addTerrain(terrainGenerator1->getRealVertArr(), 0, 0);
		PathCreator1->addTerrain(terrainGenerator2->getRealVertArr(), 0, 100);
		PathCreator1->addTerrain(terrainGenerator3->getRealVertArr(), 100, 0);
		PathCreator1->addTerrain(terrainGenerator4->getRealVertArr(), 100, 100);
		PathCreator.trumpTheBorders();

		//PathCreator.createNodes();

		// Create a Main Camera
		Camera* cam = nullptr;
		camera = gScene.createEmptyGameObject(DirectX::XMVectorSet(0, 25, 0, 0));
		camera->name = "Main Camera";
		cam = new Camera(HEIGHT, WIDTH, 70.0f, 0.01f, 1000.0f);
		camera->transform.setRotation(DirectX::XMVectorSet(0, 0, 70, 0));
		camera->addComponent(cam);
		PlayerSelectionScript* playerSelectionScript = new PlayerSelectionScript(camera);
		camera->addComponent(playerSelectionScript);

		PlayerScript *playerscript = new PlayerScript();
		camera->addComponent(playerscript);


		GameObject* cube = gScene.createEmptyGameObject(DirectX::XMVectorSet(1, 0, 1, 0));
		AudioListener* audioListener = new AudioListener();
		camera->addComponent(audioListener);
		cube->name = "Human";
		cube->tag = 1;
		AssetManager.AddMesh("Assets/Cube.obj");
		MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh(4));
		cube->addComponent(meshFilter);
		cube->addComponent(AssetManager.getMaterial(0));
		Unit *UnitHero1 = new Unit(Worker);
		cube->addComponent(UnitHero1);
		//playerscript->friendlyUnits.push_back(UnitHero1);


		GameObject* cube2 = gScene.createEmptyGameObject(DirectX::XMVectorSet(1, 0, 20, 0));
		cube2->name = "Goldmine";
		cube2->tag = 3;
		cube2->addComponent(meshFilter);
		cube2->addComponent(AssetManager.getMaterial(0));
		Unit *UnitSoldier1 = new Unit(GoldMine);
		cube2->addComponent(UnitSoldier1);

		GameObject* cube3 = gScene.createEmptyGameObject(DirectX::XMVectorSet(20, 0, 1, 0));
		cube3->name = "Bank";
		cube3->tag = 3;
		cube3->addComponent(meshFilter);
		cube3->addComponent(AssetManager.getMaterial(0));
		Unit* unitBuilding = new Unit(Bank);
		cube3->addComponent(unitBuilding);
		//playerscript->friendlyBuildings.push_back(unitBuilding);

		GameObject* cube4 = gScene.createEmptyGameObject(DirectX::XMVectorSet(10, 0, 10, 0));
		cube4->name = "Hero";
		cube4->tag = 1;
		cube4->addComponent(meshFilter);
		cube4->addComponent(AssetManager.getMaterial(0));
		Unit* UnitHero2 = new Unit(Hero);
		cube4->addComponent(UnitHero2);
		//playerscript->friendlyUnits.push_back(UnitHero2);

		
		/*ClickToMove* clickToMove = new ClickToMove(cam);
		cube->addComponent(clickToMove);*/




		Editor* editor = nullptr;
		editor = new Editor();
		editor->Start(&wndHandle, gDevice, gDeviceContext, this);

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

				OnResize();

				gScene.frustumCull(camera);
				objectsToRender = gScene.getFrustumCulledResult();
				if (editor != nullptr)
					editor->Update();
				gScene.update();

				gDeviceContext->PSSetShaderResources(0, 1, &renderManager->m_shaderResourceView);


				renderManager->EndFrame(); // END RENDERING

				objectsToRender[0].clear();

				hasResized = false;
			}
		}

		if (editor != nullptr)
			delete editor;

		delete cam;
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
	if (!hasResized)
		return;

	hasResized = false;

	Vector2 ns = Input.GetEngineWindowResolution();
	Vector2 sns = Input.GetDesktopResolution();

	Debug.Log(ns.y, "\t", sns.y);

	Input.GetEngineWindowResolution();

	int width = Input.GetWidth();
	int height = Input.GetHeight();

	//if (firstThing != 0)
		//CreateDirect3DContext(wnd);

	//firstThing = 1;



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

	//DXGI_MODE_DESC dxgiModeDesc;
	//ZeroMemory(&dxgiModeDesc, sizeof(dxgiModeDesc));
	//dxgiModeDesc.Width = (unsigned int)ns.x;
	//dxgiModeDesc.Height = (unsigned int)ns.y;
	//dxgiModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//dxgiModeDesc.RefreshRate = DXGI_RATIONAL{ m_refreshRateNumerator, m_refreshRateDenominator };
	//dxgiModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//dxgiModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//HRESULT result = gSwapChain->ResizeTarget(&dxgiModeDesc);
	//if (FAILED(result))
	//	Console.error("Failed to set resize swapchain");

	//result = gSwapChain->ResizeBuffers(1, (unsigned int)ns.x, (unsigned int)ns.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	//if (FAILED(result))
	//	Console.error("Failed to set resize buffer swapchain");

	//ID3D11Texture2D* backBuffer = nullptr;
	//result = gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBuffer));
	//if (FAILED(result))
	//	Console.error("Failed to get buffer swapchain");

	//result = gDevice->CreateRenderTargetView(backBuffer, nullptr, &gBackbufferRTV);

	//backBuffer->Release();
	//backBuffer = nullptr;

	//if (FAILED(result))
	//	Console.error("Failed to create render target view.");

	CreateDepthStencilBuffer();
	CreateDepthStencilView();

	renderManager->UpdateStuff(gDevice, gDeviceContext, gBackbufferRTV, gSwapChain, m_depthStencilView);

	renderManager->CreateRenderTarget(ns.x, ns.y);

	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();

	/*SetViewport(ns.x, ns.y);
	Console.success("Successfully Resized Window");
	std::string str = "Vector2(" + std::to_string(ns.x) + ", " + std::to_string(ns.y) + ")";
	Console.success("New Window Size: ", str);*/
}

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

	RECT rc = { 0, 0, WIDTH, HEIGHT };
	//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"dSXOv9vbM1MIm5kAf4yjjw",
		L"Aurora Engine",
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

	//gSwapChain->SetFullscreenState(TRUE, NULL);

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
