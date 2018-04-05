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

void Material::setTexture(ID3D11ShaderResourceView *textureview)
{
	this->textureview = textureview;

}

void Material::bindMaterial()
{
	shaderProgram->ActivateShader();
	gDeviceContext->PSSetShaderResources(0, 1, &textureview);
}


