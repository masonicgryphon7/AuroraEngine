#pragma once
#include "Component.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include "AssetManager.h"



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


private:
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	
	LIGHT_TYPES lightType;
	SHADOW_TYPE shadowType;
	TEXTURE_RESOLUTIONS shadowMapSize;
	
	DirectX::XMVECTOR color;
	float lightIntensity;
	float shadowStrength;
	
	void CreateRenderTarget(int width, int height);

};

