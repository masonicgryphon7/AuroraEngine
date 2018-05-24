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
	CreateInstanceMatrixBuffer();
	CreateSkeletonBuffer();
	matrixBufferData.animate = 0;
}


RenderManager::~RenderManager()
{
	m_renderTargetTexture->Release();
	m_shaderResourceView->Release();
	matrixBuffer->Release();
	instanceBuffer->Release();
	skeletonBuffer->Release();
	m_renderTargetView->Release();
}

void RenderManager::Render(GameObject * cameraObject, std::vector<GameObject*>* objectsToRender, int width, int height)
{
	DirectX::XMMATRIX viewMatrix = cameraObject->getComponent<Camera>()->calculateViewMatrix();
	DirectX::XMMATRIX perspectiveMatrix = cameraObject->getComponent<Camera>()->calculatePerspectiveMatrix();
	//Sort Objects To Render
	int instanceIndex = 0;

	//Mesh sort
	for (int i = 0; i < objectsToRender->size(); i++)
	{
		Mesh* temp = objectsToRender[0][i]->getComponent<MeshFilter>()->getMesh();
		if (temp->renderIndex == -1) {
			objectsToRender[0][i]->instanceIndex = instanceIndex;
			temp->renderIndex = instanceIndex;
			instanceIndex++;
			meshVector.push_back(temp);
		}
		else {
			objectsToRender[0][i]->instanceIndex = temp->renderIndex;
		}


	}

	//Material sort
	for (int i = 0; i < objectsToRender->size(); i++)
	{
		Material* temp = objectsToRender[0][i]->getComponent<MaterialFilter>()->material;
		if (temp->getAlpha()) {
			//Seperate translucent objects
			translucentDraw.push_back(objectsToRender[0][i]);
		}

	}

	//Create instance groups
	for (int i = 0; i < instanceIndex; i++)
	{
		std::vector<GameObject*> tempInstanceGroup;
		for (int j = 0; j < objectsToRender[0].size(); j++)
		{
			if (objectsToRender[0][j]->instanceIndex == i) {
				tempInstanceGroup.push_back(objectsToRender[0][j]);
			}
		}

		if (tempInstanceGroup.size() > 0) {
			opaqueDraw.push_back(tempInstanceGroup);
		}

	}

	//Back To Front Sort
	auto cmp = [&](GameObject*lhs, GameObject*rhs) {
		DirectX::XMVECTOR camPos = cameraObject->transform.getPosition();
		DirectX::XMVECTOR camForward = cameraObject->transform.getForward();

		float len1 = DirectX::XMVectorGetW( DirectX::XMVector3Dot(camForward, DirectX::XMVectorSubtract(camPos, lhs->transform.getPosition())));
		float len2 = DirectX::XMVectorGetW(DirectX::XMVector3Dot(camForward, DirectX::XMVectorSubtract(camPos, rhs->transform.getPosition())));

		return len1 < len2; 
	};
	std::sort(translucentDraw.begin(), translucentDraw.end(), cmp);

	////Render shadow maps
	Light* directionalLight = cameraObject->getComponent<Light>();
	RenderShadowMaps(directionalLight, width, height);
	gDeviceContext->PSSetShaderResources(16, 1, directionalLight->getID3D11ShaderResourceView());


	//Draw Opaque
	for (int i = 0; i < opaqueDraw.size(); i++)
	{

		opaqueDraw[i][0]->materialFilterComponent->material->bindMaterial();
		matrixBufferData.xMaterialTile = opaqueDraw[i][0]->materialFilterComponent->material->getXTile();
		matrixBufferData.yMaterialTile = opaqueDraw[i][0]->materialFilterComponent->material->getYTile();

		opaqueDraw[i][0]->meshFilterComponent->getMesh()->bindMesh();

		//Fill matrixbuffer
		matrixBufferData.isTerrain = opaqueDraw[i][0]->materialFilterComponent->material->isTerrain();
		DirectX::XMStoreFloat4x4(&matrixBufferData.world, DirectX::XMMatrixTranspose(opaqueDraw[i][0]->calculateWorldMatrix()));
		DirectX::XMStoreFloat4x4(&matrixBufferData.view, DirectX::XMMatrixTranspose(viewMatrix));
		DirectX::XMStoreFloat4x4(&matrixBufferData.projection, DirectX::XMMatrixTranspose(perspectiveMatrix));
		DirectX::XMStoreFloat4x4(&matrixBufferData.lightProjection, DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(directionalLight->calculateViewMatrix(), directionalLight->calculatePerspectiveMatrix(width,height))));
		DirectX::XMStoreFloat4(&matrixBufferData.cameraPosition, cameraObject->transform.getPosition());
		Animator* animator = opaqueDraw[i][0]->getComponent<Animator>();
		if (animator) {
			matrixBufferData.animate = 1;
			std::vector<DirectX::XMMATRIX> skel = animator->getMatrixPalette();
			for (int j = 0; j < skel.size(); j++)
			{
				DirectX::XMStoreFloat4x4( &skeleton[j], skel[j]);
				//DirectX::XMStoreFloat4x4( &skeleton[j], DirectX::XMMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
			}
			//DirectX::XMStoreFloat4x4(&skeleton[1], DirectX::XMMatrixTranspose( DirectX::XMMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f, 4.0f, 1.0f)));
			//DirectX::XMStoreFloat4x4(&skeleton[2], DirectX::XMMatrixTranspose(DirectX::XMMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 8.0f, 0.0f, 8.0f, 1.0f)));

			gDeviceContext->UpdateSubresource(skeletonBuffer, 0, nullptr, &skeleton, 0, 0);

		}
		else
		{
			matrixBufferData.animate = 0;
		}
		gDeviceContext->UpdateSubresource(matrixBuffer, 0, nullptr, &matrixBufferData, 0, 0);

		//Instancing
		int nrOfObjectsDrawn = 0;
		while (nrOfObjectsDrawn!=opaqueDraw[i].size())
		{
			//Get world matricies
			int nrToDraw = 0;
			nrToDraw = opaqueDraw[i].size() - nrOfObjectsDrawn;
			if (nrToDraw > 100)
				nrToDraw = 100;

			for (int j = 0; j < nrToDraw; j++)
			{
				DirectX::XMFLOAT4X4 temp;
				DirectX::XMStoreFloat4x4(&temp, DirectX::XMMatrixTranspose(opaqueDraw[i][j+nrOfObjectsDrawn]->calculateWorldMatrix()));
				InstanceMatrixData.instanceTransforms[j]=temp;
				InstanceMatrixData.unitTag[j].x = opaqueDraw[i][j + nrOfObjectsDrawn]->tag;
				int jj = 0;
			}
	
			nrOfObjectsDrawn += nrToDraw;
			gDeviceContext->UpdateSubresource(instanceBuffer, 0, nullptr, &InstanceMatrixData, 0, 0);
			
			
			
			//Get skeleton
			
			gDeviceContext->DrawInstanced(opaqueDraw[i][0]->meshFilterComponent->getMesh()->getVertexCount(), opaqueDraw[i].size(), 0, 0);

		}		
	}
	//Clean for shadowmap
	gDeviceContext->PSSetShaderResources(16, 1, nullSRV);

	//Translucent
	for (int i = 0; i < translucentDraw.size(); i++)
	{
		translucentDraw[i]->materialFilterComponent->material->bindMaterial();
		matrixBufferData.xMaterialTile = translucentDraw[i]->materialFilterComponent->material->getXTile();
		matrixBufferData.yMaterialTile = translucentDraw[i]->materialFilterComponent->material->getYTile();

		translucentDraw[i]->meshFilterComponent->getMesh()->bindMesh();

		//Fill matrixbuffer
		matrixBufferData.isTerrain = translucentDraw[i]->materialFilterComponent->material->isTerrain();
		DirectX::XMStoreFloat4x4(&matrixBufferData.world, DirectX::XMMatrixTranspose(translucentDraw[i]->calculateWorldMatrix()));
		DirectX::XMStoreFloat4x4(&matrixBufferData.view, DirectX::XMMatrixTranspose(viewMatrix));
		DirectX::XMStoreFloat4x4(&matrixBufferData.projection, DirectX::XMMatrixTranspose(perspectiveMatrix));
		DirectX::XMStoreFloat4x4(&matrixBufferData.lightProjection, DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(directionalLight->calculateViewMatrix(), directionalLight->calculatePerspectiveMatrix(width, height))));
		DirectX::XMStoreFloat4(&matrixBufferData.cameraPosition, cameraObject->transform.getPosition());
		gDeviceContext->UpdateSubresource(matrixBuffer, 0, nullptr, &matrixBufferData, 0, 0);

		//transform
		DirectX::XMFLOAT4X4 temp;
		DirectX::XMStoreFloat4x4(&temp, DirectX::XMMatrixTranspose(translucentDraw[i]->calculateWorldMatrix()));
		InstanceMatrixData.instanceTransforms[0] = temp;
		gDeviceContext->UpdateSubresource(instanceBuffer, 0, nullptr, &InstanceMatrixData, 0, 0);

		gDeviceContext->Draw(translucentDraw[i]->meshFilterComponent->getMesh()->getVertexCount(), 0);

	}



	//Clean
	for (int i = 0; i < meshVector.size(); i++)
	{
		meshVector[i]->renderIndex = -1;
	}
	for (int i = 0; i < materialVector.size(); i++)
	{
		materialVector[i]->renderIndex = -1;
	}
	for (int i = 0; i < objectsToRender->size(); i++)
	{
		objectsToRender[0][i]->instanceIndex = -1;
	}
	meshVector.clear();
	materialVector.clear();
	opaqueDraw.clear();
	translucentDraw.clear();
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
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	int gg = sizeof(MatrixBufferStruct);
	bufferDesc.ByteWidth = sizeof(MatrixBufferStruct);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// check if the creation failed for any reason
	HRESULT hr = 0;
	hr = gDevice->CreateBuffer(&bufferDesc, nullptr, &matrixBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
	gDeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
	gDeviceContext->PSSetConstantBuffers(0, 1, &matrixBuffer);


}

void RenderManager::CreateInstanceMatrixBuffer()
{
	// initialize the description of the buffer.
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(InstanceMatrixStruct);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// check if the creation failed for any reason
	HRESULT hr = 0;
	hr = gDevice->CreateBuffer(&bufferDesc, nullptr, &instanceBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
	gDeviceContext->VSSetConstantBuffers(1, 1, &instanceBuffer);
	gDeviceContext->PSSetConstantBuffers(2, 1, &instanceBuffer);
}

void RenderManager::CreateSkeletonBuffer()
{
	// initialize the description of the buffer.
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * 20;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// check if the creation failed for any reason
	HRESULT hr = 0;
	hr = gDevice->CreateBuffer(&bufferDesc, nullptr, &skeletonBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
	gDeviceContext->VSSetConstantBuffers(2, 1, &skeletonBuffer);
}



void RenderManager::RenderShadowMaps(Light * light, int width, int height)
{
	ID3D11RenderTargetView* gBackbufferRTVgg = nullptr;
	ID3D11DepthStencilView* m_depthStencilViewgg = nullptr;
	gDeviceContext->OMGetRenderTargets(1, &gBackbufferRTVgg, &m_depthStencilViewgg);
	D3D11_VIEWPORT prevVp;
	UINT num = 1;
	gDeviceContext->RSGetViewports(&num, &prevVp);
	DirectX::XMMATRIX viewMatrix = light->calculateViewMatrix();
	DirectX::XMMATRIX perspectiveMatrix = light->calculatePerspectiveMatrix(width, height);
	gDeviceContext->ClearDepthStencilView(light->getID3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	gDeviceContext->OMSetRenderTargets(1, light->getID3D11RenderTargetView(), light->getID3D11DepthStencilView());
	D3D11_VIEWPORT vp;
	vp.Width = (float)light->widht;
	vp.Height = (float)light->height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
	//light->pixelShader->ActivateShader();
	gDeviceContext->PSSetShader(nullptr, nullptr, 0);
	float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };



	//Draw Opaque
	for (int i = 0; i < opaqueDraw.size(); i++)
	{
		opaqueDraw[i][0]->meshFilterComponent->getMesh()->bindMesh();

		//Fill matrixbuffer
		DirectX::XMStoreFloat4x4(&matrixBufferData.world, DirectX::XMMatrixTranspose(opaqueDraw[i][0]->calculateWorldMatrix()));
		DirectX::XMStoreFloat4x4(&matrixBufferData.view, DirectX::XMMatrixTranspose(viewMatrix));
		DirectX::XMStoreFloat4x4(&matrixBufferData.projection, DirectX::XMMatrixTranspose(perspectiveMatrix));

		Animator* animator = opaqueDraw[i][0]->getComponent<Animator>();
		if (animator) {
			matrixBufferData.animate = 1;
			std::vector<DirectX::XMMATRIX> skel = animator->getMatrixPalette();
			for (int j = 0; j < skel.size(); j++)
			{
				DirectX::XMStoreFloat4x4(&skeleton[j], skel[j]);
				//DirectX::XMStoreFloat4x4(&skeleton[j], DirectX::XMMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
			}
			//DirectX::XMStoreFloat4x4(&skeleton[1], DirectX::XMMatrixTranspose(DirectX::XMMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f, 4.0f, 1.0f)));
			//DirectX::XMStoreFloat4x4(&skeleton[2], DirectX::XMMatrixTranspose(DirectX::XMMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 8.0f, 0.0f, 8.0f, 1.0f)));

			gDeviceContext->UpdateSubresource(skeletonBuffer, 0, nullptr, &skeleton, 0, 0);

		}
		else
		{
			matrixBufferData.animate = 0;
		}
		gDeviceContext->UpdateSubresource(matrixBuffer, 0, nullptr, &matrixBufferData, 0, 0);

		//Instancing
		int nrOfObjectsDrawn = 0;
		while (nrOfObjectsDrawn != opaqueDraw[i].size())
		{
			//Get world matricies
			int nrToDraw = 0;
			nrToDraw = opaqueDraw[i].size() - nrOfObjectsDrawn;
			if (nrToDraw > 100)
				nrToDraw = 100;

			for (int j = 0; j < nrToDraw; j++)
			{
				DirectX::XMFLOAT4X4 temp;
				DirectX::XMStoreFloat4x4(&temp, DirectX::XMMatrixTranspose(opaqueDraw[i][j + nrOfObjectsDrawn]->calculateWorldMatrix()));
				InstanceMatrixData.instanceTransforms[j] = temp;
				InstanceMatrixData.unitTag[j].x = opaqueDraw[i][j + nrOfObjectsDrawn]->tag;
				int jj = 0;
			}

			nrOfObjectsDrawn += nrToDraw;
			gDeviceContext->UpdateSubresource(instanceBuffer, 0, nullptr, &InstanceMatrixData, 0, 0);



			//Get skeleton

			gDeviceContext->DrawInstanced(opaqueDraw[i][0]->meshFilterComponent->getMesh()->getVertexCount(), opaqueDraw[i].size(), 0, 0);

		}
	}


	//Translucent
	for (int i = 0; i < translucentDraw.size(); i++)
	{
		translucentDraw[i]->materialFilterComponent->material->bindMaterial();
		matrixBufferData.xMaterialTile = translucentDraw[i]->materialFilterComponent->material->getXTile();
		matrixBufferData.yMaterialTile = translucentDraw[i]->materialFilterComponent->material->getYTile();

		translucentDraw[i]->meshFilterComponent->getMesh()->bindMesh();

		//Fill matrixbuffer
		DirectX::XMStoreFloat4x4(&matrixBufferData.world, DirectX::XMMatrixTranspose(translucentDraw[i]->calculateWorldMatrix()));
		DirectX::XMStoreFloat4x4(&matrixBufferData.view, DirectX::XMMatrixTranspose(viewMatrix));
		DirectX::XMStoreFloat4x4(&matrixBufferData.projection, DirectX::XMMatrixTranspose(perspectiveMatrix));
		gDeviceContext->UpdateSubresource(matrixBuffer, 0, nullptr, &matrixBufferData, 0, 0);

		//transform
		DirectX::XMFLOAT4X4 temp;
		DirectX::XMStoreFloat4x4(&temp, DirectX::XMMatrixTranspose(translucentDraw[i]->calculateWorldMatrix()));
		InstanceMatrixData.instanceTransforms[0] = temp;
		gDeviceContext->UpdateSubresource(instanceBuffer, 0, nullptr, &InstanceMatrixData, 0, 0);

		gDeviceContext->Draw(translucentDraw[i]->meshFilterComponent->getMesh()->getVertexCount(), 0);

	}
	gDeviceContext->RSSetViewports(1, &prevVp);
	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTVgg, m_depthStencilViewgg);

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
	float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };

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