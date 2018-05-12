#include "Light.h"




Light::Light(LIGHT_TYPES lightType, SHADOW_TYPE shadowType, TEXTURE_RESOLUTIONS shadowMapSize, DirectX::XMVECTOR color, float lightIntensity, float shadowStrength)
{
	this->lightType = lightType;
	this->shadowType = shadowType;
	this->shadowMapSize = shadowMapSize;
	this->color = color;
	this->lightIntensity = lightIntensity;
	this->shadowStrength = shadowStrength;

	setShadowMapSize(this->shadowMapSize);
}

Light::~Light()
{
	m_depthStencilView->Release();

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
	CreateRenderTarget(x, y);
}

void Light::CreateRenderTarget(int width, int height)
{


}
