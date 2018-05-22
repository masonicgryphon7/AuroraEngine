#pragma once
#include "GameObject.h"
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Camera.h"
#include "Animator.h"
#include"Light.h"
#include "Scene.h"
#define NO_VSYNC 0
#define VSYNC_1_FRAME 1
#define VSYNC_2_FRAME 2

struct MatrixBufferStruct
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4X4 lightProjection;
	DirectX::XMFLOAT4 cameraPosition;
	int isTerrain;
	float xMaterialTile;
	float yMaterialTile;

	int animate;
};

struct InstanceMatrixStruct
{
	DirectX::XMFLOAT4X4 instanceTransforms[100]{ DirectX::XMFLOAT4X4() };
	DirectX::XMINT4 unitTag[100]{ DirectX::XMINT4(0,0,0,0) };
};

class RenderManager
{
public:
	RenderManager();
	RenderManager(ID3D11Device * gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11RenderTargetView* gBackbufferRTV, IDXGISwapChain *swapChain, ID3D11DepthStencilView* depth);

	~RenderManager();

	void Render(GameObject* cameraObject, std::vector<GameObject*>* objectsToRender, int width, int height);
	void UpdateStuff(ID3D11Device * gDevice, ID3D11DeviceContext* gDeviceContext, ID3D11RenderTargetView* gBackbufferRTV, IDXGISwapChain *swapChain, ID3D11DepthStencilView* depth);


private:
	ID3D11ShaderResourceView * nullSRV[1] = { nullptr };

	ID3D11Device * gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11RenderTargetView* gBackbufferRTV = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11Buffer* matrixBuffer = nullptr;
	ID3D11Buffer* instanceBuffer = nullptr;
	ID3D11Buffer* skeletonBuffer = nullptr;
	MatrixBufferStruct matrixBufferData;
	InstanceMatrixStruct InstanceMatrixData;
	void CreateMatrixBuffer();
	void CreateInstanceMatrixBuffer();
	void CreateSkeletonBuffer();

	std::vector<std::vector<GameObject*>> opaqueDraw;
	std::vector<GameObject*> translucentDraw;
	std::vector<Mesh*> meshVector;
	std::vector<Material*> materialVector;
	DirectX::XMFLOAT4X4 skeleton[20]{ DirectX::XMFLOAT4X4() };

	void RenderShadowMaps(Light* lightn, int width, int height);
	// for render texture
public:
	void CreateRenderTarget(int width, int height);

	void SetRenderTarget(ID3D11DepthStencilView* depthStencilView);
	void ClearRenderTarget(ID3D11DepthStencilView* depthStencilView);

	void BeginFrame();
	void EndFrame();

	//private:
	ID3D11Texture2D * m_renderTargetTexture = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
};

