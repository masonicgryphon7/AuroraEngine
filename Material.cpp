#include "Material.h"



Material::Material()
{}

Material::Material(std::string name, ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, ShaderProgram * pixelShader)
{
	this->materialName = name;
	this->gDeviceContext = gDeviceContext;
	this->gDevice = gDevice;
	this->pixelShader = pixelShader;
	createSamplerState();
	createSamplerStateForIDMap();
}


Material::~Material()
{
	m_sampleState->Release();
	m_sampleStateIDMAP->Release();
}

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

void Material::setTeamIdMap(ID3D11ShaderResourceView * teamIdMap)
{
	this->teamIdMap = teamIdMap;
}

void Material::setTerrainMaterials(ID3D11ShaderResourceView * albedo1, ID3D11ShaderResourceView * normal1, ID3D11ShaderResourceView * AORoughMet1, ID3D11ShaderResourceView * albedo2, ID3D11ShaderResourceView * normal2, ID3D11ShaderResourceView * AORoughMet2, ID3D11ShaderResourceView * albedo3, ID3D11ShaderResourceView * normal3, ID3D11ShaderResourceView * AORoughMet3, ID3D11ShaderResourceView *albedo4, ID3D11ShaderResourceView *normal4, ID3D11ShaderResourceView *AORoughMet4, ID3D11ShaderResourceView * ID_MAP)
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

	this->TerrainAlbedo_4 = albedo4;
	this->TerrainNormal_4 = normal4;
	this->TerrainAORoughMet_4 = AORoughMet4;

	this->ID_MAP = ID_MAP;
}

void Material::bindMaterial()
{
	pixelShader->ActivateShader();
	gDeviceContext->PSSetSamplers(0, 1, &m_sampleState);
	gDeviceContext->PSSetSamplers(1, 1, &m_sampleStateIDMAP);

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

	gDeviceContext->PSSetShaderResources(13, 1, &TerrainAlbedo_4);
	gDeviceContext->PSSetShaderResources(14, 1, &TerrainNormal_4);
	gDeviceContext->PSSetShaderResources(15, 1, &TerrainAORoughMet_4);

	gDeviceContext->PSSetShaderResources(17, 1, &teamIdMap);

}

void Material::update()
{
}

const std::string Material::getMaterialName() const
{
	return materialName;
}



void Material::createSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	gDevice->CreateSamplerState(&samplerDesc, &m_sampleState);

}

void Material::createSamplerStateForIDMap()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	gDevice->CreateSamplerState(&samplerDesc, &m_sampleStateIDMAP);
}


