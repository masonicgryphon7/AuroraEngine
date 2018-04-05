#include "Material.h"



Material::Material()
{}

Material::Material(ID3D11DeviceContext* gDeviceContext, ShaderProgram * shaderProgram) :Component(-1, "Material")
{
	this->gDeviceContext = gDeviceContext;
	this->shaderProgram = shaderProgram;
}


Material::~Material()
{}

void Material::setAlbedo(ID3D11ShaderResourceView *albedo)
{
	this->albedo = albedo;
}

void Material::setNormal(ID3D11ShaderResourceView * normal)
{
	this->normal = normal;
}

void Material::bindMaterial()
{
	shaderProgram->ActivateShader();
	gDeviceContext->PSSetShaderResources(0, 1, &albedo);
	gDeviceContext->PSSetShaderResources(1, 1, &normal);
}


