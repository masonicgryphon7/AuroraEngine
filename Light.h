#pragma once
#include "Component.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include "AssetManager.h"
#include "ShaderProgram.h"


enum  class LIGHT_TYPES
{
	Spot, Directional, Point
};

enum  class SHADOW_TYPE
{
	NoShadows, HardShadow, SoftShadows
};

enum class TEXTURE_RESOLUTIONS
{
	R256x256, R512x512, R1024x1024, R2048x2048
};


class Light :
	public Component
{
public:
	Light(LIGHT_TYPES lightType, SHADOW_TYPE shadowType, TEXTURE_RESOLUTIONS shadowMapSize, DirectX::XMVECTOR color, float lightIntensity, float shadowStrength);
	~Light();

	void setShadowMapSize(TEXTURE_RESOLUTIONS shadowMapSize);
	DirectX::XMMATRIX calculateViewMatrix();
	DirectX::XMMATRIX calculatePerspectiveMatrix(int width, int height);
	ShaderProgram* pixelShader = nullptr;

	SHADOW_TYPE getShadowType() { return shadowType; };
	ID3D11ShaderResourceView** getID3D11ShaderResourceView() { return &m_shadowMapView; };
	ID3D11DepthStencilView* getID3D11DepthStencilView() { return m_depthStencilView; };
	ID3D11RenderTargetView** getID3D11RenderTargetView() { return &m_renderTargetView; };

	int widht = 0;
	int height = 0;
private:
	ID3D11RenderTargetView * m_renderTargetView = nullptr;
	ID3D11ShaderResourceView * m_shadowMapView = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D* m_depthStencilBuffer = nullptr;
	ID3D11Texture2D* m_renderTargetTexture = nullptr;
	LIGHT_TYPES lightType;
	SHADOW_TYPE shadowType;
	TEXTURE_RESOLUTIONS shadowMapSize;
	
	DirectX::XMVECTOR color;
	float lightIntensity;
	float shadowStrength;
	float nearZ;
	float farZ;
	void CreateDepthStencilBuffer(int width, int height);
};

