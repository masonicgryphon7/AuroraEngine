#include "RenderManager.h"
#include "Console.h"
#include "ImGuizmo.h"
#include "InputHandler.h"

void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float *m16);
void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float *m16);

RenderManager::RenderManager()
{}

RenderManager::RenderManager(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, ID3D11RenderTargetView * gBackbufferRTV, IDXGISwapChain *swapChain, ID3D11DepthStencilView* depth)
{
	this->gDevice = gDevice;
	this->gBackbufferRTV = gBackbufferRTV;
	this->gDeviceContext = gDeviceContext;
	this->gSwapChain = swapChain;
	this->m_depthStencilView = depth;
	CreateMatrixBuffer();
}


RenderManager::~RenderManager()
{
	m_renderTargetTexture->Release();
	m_shaderResourceView->Release();
	matrixBuffer->Release();
	m_renderTargetView->Release();
}

void RenderManager::ForwardRender(GameObject * cameraObject, std::vector<GameObject*> objectsToRender)
{
	DirectX::XMMATRIX viewMatrix = cameraObject->getComponent<Camera>()->calculateViewMatrix();
	DirectX::XMMATRIX perspectiveMatrix = cameraObject->getComponent<Camera>()->calculatePerspectiveMatrix();


	//for each 
	for (int i = 0; i < objectsToRender.size(); i++)
	{

		objectsToRender[i]->materialComponent->bindMaterial();
		objectsToRender[i]->meshFilterComponent->getMesh()->bindMesh();


		//Fill matrixbuffer
		D3D11_MAPPED_SUBRESOURCE dataPtr;
		gDeviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		DirectX::XMStoreFloat4x4(&matrixBufferData.world, DirectX::XMMatrixTranspose(objectsToRender[i]->calculateWorldMatrix()));
		DirectX::XMStoreFloat4x4(&matrixBufferData.view, DirectX::XMMatrixTranspose(viewMatrix));
		DirectX::XMStoreFloat4x4(&matrixBufferData.projection, DirectX::XMMatrixTranspose(perspectiveMatrix));
		DirectX::XMStoreFloat4(&matrixBufferData.cameraPosition, cameraObject->transform.getPosition());
		memcpy(dataPtr.pData, &matrixBufferData, sizeof(MatrixBufferStruct));
		gDeviceContext->Unmap(matrixBuffer, 0);
		gDeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);


		// issue a draw call of 3 vertices (similar to OpenGL)

		gDeviceContext->Draw(objectsToRender[i]->meshFilterComponent->getMesh()->getVertexCount(), 0);

	}

	/*ImGuizmo::BeginFrame();
	ImGuizmo::SetDrawlist();*/
	//ImGuizmo::DrawCube(cameraView, cameraProjection, objectMatrix);
}

void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float *m16)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0f * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	m16[0] = temp / temp2;
	m16[1] = 0.0;
	m16[2] = 0.0;
	m16[3] = 0.0;
	m16[4] = 0.0;
	m16[5] = temp / temp3;
	m16[6] = 0.0;
	m16[7] = 0.0;
	m16[8] = (right + left) / temp2;
	m16[9] = (top + bottom) / temp3;
	m16[10] = (-zfar - znear) / temp4;
	m16[11] = -1.0f;
	m16[12] = 0.0;
	m16[13] = 0.0;
	m16[14] = (-temp * zfar) / temp4;
	m16[15] = 0.0;
}

void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float *m16)
{
	float ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
	xmax = ymax * aspectRatio;
	Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
}



void RenderManager::UpdateStuff(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, ID3D11RenderTargetView * gBackbufferRTV, IDXGISwapChain * swapChain, ID3D11DepthStencilView * depth)
{
	this->gDevice = gDevice;
	this->gBackbufferRTV = gBackbufferRTV;
	this->gDeviceContext = gDeviceContext;
	this->gSwapChain = swapChain;
	this->m_depthStencilView = depth;
}

void RenderManager::CreateMatrixBuffer()
{
	// initialize the description of the buffer.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(MatrixBufferStruct);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// check if the creation failed for any reason
	HRESULT hr = 0;
	hr = gDevice->CreateBuffer(&bufferDesc, nullptr, &matrixBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
}

void RenderManager::CreateRenderTarget(int width, int height)
{
	if (m_renderTargetTexture != nullptr)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_shaderResourceView)
	{
		m_shaderResourceView->Release();
		m_shaderResourceView = nullptr;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// TEXTURE
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	gDevice->CreateTexture2D(&textureDesc, nullptr, &m_renderTargetTexture);


	// RENDER TARGET VIEW
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	gDevice->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);


	// SHADER RESOURCE VIEW
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	gDevice->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);

	//Console.success("Passed");
}

void RenderManager::SetRenderTarget(ID3D11DepthStencilView * depthStencilView)
{
	gDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
}

void RenderManager::ClearRenderTarget(ID3D11DepthStencilView * depthStencilView)
{
	float clearColor[] = { 1.0f, 0.28f, 0.28f, 1.0f };

	gDeviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	gDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);

}

void RenderManager::BeginFrame()
{
	float clearColor[] = { 0.28f, 0.28f, 0.28f, 1.0f };

	//// use DeviceContext to talk to the API
	////gDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	//gDeviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	//gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
	//gDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	gDeviceContext->PSSetShaderResources(0, 1, nullptr);

	//gDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	gDeviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	gDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
}

void RenderManager::EndFrame()
{
	// NO_VSYNC == no vsync (free)
	// VSYNC_1_FRAME == vsync (1frame) { 60FPS }
	// VSYNC_2_FRAME == vsync (2frame) { 30FPS }

	gSwapChain->Present(NO_VSYNC, 0);
}
