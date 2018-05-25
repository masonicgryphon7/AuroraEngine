#include "Light.h"
#include "GameObject.h"



Light::Light(LIGHT_TYPES lightType, SHADOW_TYPE shadowType, TEXTURE_RESOLUTIONS shadowMapSize, DirectX::XMVECTOR color, float lightIntensity, float shadowStrength)
{
	this->lightType = lightType;
	this->shadowType = shadowType;
	this->shadowMapSize = shadowMapSize;
	this->color = color;
	this->lightIntensity = lightIntensity;
	this->shadowStrength = shadowStrength;
	nearZ = 0.01;
	farZ = 400;
	setShadowMapSize(this->shadowMapSize);
}

Light::~Light()
{
	m_depthStencilView->Release();
	m_depthStencilBuffer->Release();
	m_shadowMapView->Release();
	m_renderTargetTexture -> Release();
	m_renderTargetView->Release();
	
}

void Light::setShadowMapSize(TEXTURE_RESOLUTIONS shadowMapSize)
{
	int x = 0;

	switch (shadowMapSize)
	{
	case TEXTURE_RESOLUTIONS::R256x256:
		x = 256;
		break;
	case TEXTURE_RESOLUTIONS::R512x512:
		x = 512;
		break;
	case TEXTURE_RESOLUTIONS::R1024x1024:
		x = 1024;
		break;
	case TEXTURE_RESOLUTIONS::R2048x2048:
		x = 2048;
		break;
	default:
		break;
	}

	int y = x;
	widht = x;
	height = y;
	CreateDepthStencilBuffer(x, y);
}


DirectX::XMMATRIX Light::calculateViewMatrix()
{
	//return DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(20.0f, 50.0f, 20.0f, 1.0f), DirectX::XMVectorSet(25.0f, 0.0f, 25.0f, 1.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	return DirectX::XMMatrixLookToLH(gameObject->transform.getPosition(), gameObject->transform.getForward(), gameObject->transform.getUp());
}

DirectX::XMMATRIX  Light::calculatePerspectiveMatrix(int width, int height)
{
	return DirectX::XMMatrixOrthographicLH(150, 150, nearZ, farZ);
}


void Light::CreateDepthStencilBuffer(int width, int height)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&m_depthStencilBuffer, sizeof(m_depthStencilBuffer));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	HRESULT result = AssetManager.getD3D11Device()->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);


	//D3D11_DEPTH_STENCIL_VIEW_DESC
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = AssetManager.getD3D11Device()->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// SHADER RESOURCE VIEW
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result=AssetManager.getD3D11Device()->CreateShaderResourceView(m_depthStencilBuffer, &shaderResourceViewDesc, &m_shadowMapView);


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_TEXTURE2D_DESC textureDesc;
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
	AssetManager.getD3D11Device()->CreateTexture2D(&textureDesc, nullptr, &m_renderTargetTexture);


	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;


	result = AssetManager.getD3D11Device()->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
}


