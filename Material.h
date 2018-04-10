#pragma once
#include "Component.h"
#include "ShaderProgram.h"

class Material : public Component
{
public:
	Material();
	Material(ID3D11DeviceContext* gDeviceContext, ShaderProgram* shaderProgram);
	~Material();

	void setAlbedo(ID3D11ShaderResourceView *albedo);
	void setNormal(ID3D11ShaderResourceView *normal);
	void setPbr(ID3D11ShaderResourceView *pbr);
	void bindMaterial();

	void update();
private:
	ID3D11ShaderResourceView * albedo = nullptr;
	ID3D11ShaderResourceView * normal = nullptr;
	ID3D11ShaderResourceView * pbr = nullptr;
	ShaderProgram* shaderProgram = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
};

