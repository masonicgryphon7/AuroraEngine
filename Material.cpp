#include "Material.h"



Material::Material()
{}

Material::Material(ID3D11DeviceContext* gDeviceContext, ShaderProgram * pixelShader)
{
	this->gDeviceContext = gDeviceContext;
	this->pixelShader = pixelShader;
}


Material::~Material()
{}

bool Material::isTerrain()
{
	return this->terrain;
}

void Material::setIsTerrain(bool YesNo)
{
	this->terrain = YesNo;
}

void Material::setAlbedo(ID3D11ShaderResourceView *albedo)
{
	this->albedo = albedo;
}

void Material::setNormal(ID3D11ShaderResourceView * normal)
{
	this->normal = normal;
}

void Material::setAORoughMet(ID3D11ShaderResourceView * AORoughMet)
{
	this->AORoughMet = AORoughMet;
}

void Material::setTerrainMaterials(ID3D11ShaderResourceView * albedo1, ID3D11ShaderResourceView * normal1, ID3D11ShaderResourceView * AORoughMet1, ID3D11ShaderResourceView * albedo2, ID3D11ShaderResourceView * normal2, ID3D11ShaderResourceView * AORoughMet2, ID3D11ShaderResourceView * albedo3, ID3D11ShaderResourceView * normal3, ID3D11ShaderResourceView * AORoughMet3, ID3D11ShaderResourceView * ID_MAP)
{
	this->TerrainAlbedo_1 = albedo1;
	this->TerrainNormal_1 = normal1;
	this->TerrainAORoughMet_1 = AORoughMet1;

	this->TerrainAlbedo_2 = albedo2;
	this->TerrainNormal_2 = normal2;
	this->TerrainAORoughMet_2 = AORoughMet2;

	this->TerrainAlbedo_3 = albedo3;
	this->TerrainNormal_3 = normal3;
	this->TerrainAORoughMet_3 = AORoughMet3;

	this->ID_MAP = ID_MAP;
}

void Material::bindMaterial()
{
	pixelShader->ActivateShader();
	gDeviceContext->PSSetShaderResources(0, 1, &albedo);
	gDeviceContext->PSSetShaderResources(1, 1, &normal);
	gDeviceContext->PSSetShaderResources(2, 1, &AORoughMet);

	gDeviceContext->PSSetShaderResources(3, 1, &TerrainAlbedo_1);
	gDeviceContext->PSSetShaderResources(6, 1, &TerrainAlbedo_2);
	gDeviceContext->PSSetShaderResources(9, 1, &TerrainAlbedo_3);

	gDeviceContext->PSSetShaderResources(4, 1, &TerrainNormal_1);
	gDeviceContext->PSSetShaderResources(7, 1, &TerrainNormal_2);
	gDeviceContext->PSSetShaderResources(10, 1, &TerrainNormal_3);

	gDeviceContext->PSSetShaderResources(5, 1, &TerrainAORoughMet_1);
	gDeviceContext->PSSetShaderResources(8, 1, &TerrainAORoughMet_2);
	gDeviceContext->PSSetShaderResources(11, 1, &TerrainAORoughMet_3);

	gDeviceContext->PSSetShaderResources(12, 1, &ID_MAP);
}

void Material::update()
{
}


