#pragma once
#include "Component.h"
#include "ShaderProgram.h"

class Material : public Component
{
public:
	Material();
	Material(ID3D11DeviceContext* gDeviceContext, ShaderProgram* shaderProgram);
	~Material();

	void setTexture(ID3D11ShaderResourceView *textureview);
	void bindMaterial();

private:
	ID3D11ShaderResourceView * textureview = nullptr;
	ShaderProgram* shaderProgram = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
};

