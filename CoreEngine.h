#pragma once

#define NOMINMAX

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "PRIMITIVE_GEOMETRY.h"
#include <stdio.h>
#include <string>
#include <ctime>
#include <algorithm>  
#include <vector>
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "TerrainGenerator.h"
#include "EditorSceneSelectionScript.h"
#include "ClickToMove.h"
#include "PlayerScript.h"
#include "PathCreator.h"
//#include "MaterialLib.h"
//#include "TextureLib.h"
//#include "MeshLib.h"
#include"AssetManager.h"
#include"Scene.h"
#include "RenderManager.h"
//#include"Transform.h"
#include "MeshFilter.h"
#include "Material.h"
#include "EditorMoveScript.h"
#include "PlayerMoveScript.h"
//#include "CharacterMovement.h"
#include "Camera.h"
#include "Time.h"
//#include "Light.h"
//#include "LIGHT_TYPES.h"
#include "InputHandler.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

#define _CRTDBG_MAP_ALLOC
class CoreEngine
{
public:
	CoreEngine(bool includeDebugConsole = false);
	~CoreEngine();

	MSG Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
	void importOBJFile(std::string filepath, unsigned int shaderProgram, int &meshID, int &materialID);

protected:
	bool consoleInitialized = false;

private:
	HINSTANCE hInstance;
	HWND wnd;
	int nCmdShow;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11Device* gDevice = nullptr;
	//ID3D11DeviceContext* gDeviceContext = nullptr;
	//ID3D11RenderTargetView* gBackbufferRTV = nullptr;
	ShaderProgram shaderProgram;
	ID3D11Buffer* gVertexBuffer = nullptr;
	ID3D11ShaderResourceView *textureview = nullptr;
	ID3D11InputLayout* gVertexLayout = nullptr;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthStencilStateEnabled;
	ID3D11DepthStencilState* m_depthStencilStateDisabled;
	//ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterizerState;
	ID3D11RasterizerState* m_rasterStateCullFront;
	ID3D11RasterizerState* m_rasterStateCullBack;
	ID3D11RasterizerState* m_rasterStateCullNone;
	ID3D11BlendState* m_alphaEnableBlendState, * m_alphaDisabledBlendState;
	static bool hasResized;
	int firstThing = 0;
	unsigned int numModes, m_refreshRateNumerator, m_refreshRateDenominator;

	//cTime engineTime;

	//AssetManager assetManager;
	//MaterialLib materialLibrary;
	//TextureLib textureLibrary;
	//MeshLib meshLibrary;
	//std::vector<GameObject*> lightsToRender;
	//std::vector<GameObject*>objectsToRender;
	//std::vector<GameObject*>sceneObjects;

	HWND InitWindow(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT CreateDirect3DContext(HWND wndHandle);
	bool CreateDepthStencilBuffer();
	bool CreateDepthStencilView();
	bool CreateDepthStencilState(void* ds, bool enabled, bool writeEnabled);
	bool CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** rasterizer);

	//void SetViewport(int x = WIDTH, int y = HEIGHT);

	void CreateTriangleData();

public:
	ID3D11DeviceContext * gDeviceContext = nullptr;
	ID3D11RenderTargetView* gBackbufferRTV = nullptr;

	ID3D11DepthStencilView * m_depthStencilView;

	std::vector<GameObject*>objectsToRender;

	GameObject* camera;
	cAssetManager assetManager;
	RenderManager *renderManager;

	void SetViewport(int x = WIDTH, int y = HEIGHT);
	void OnResize();

};
