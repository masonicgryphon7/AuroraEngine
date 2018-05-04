#pragma once
#include "ShaderProgram.h"

class Material
{
public:
	Material();
	Material(ID3D11DeviceContext* gDeviceContext, ShaderProgram* shaderProgram);
	~Material();

	void setAlpha(bool value) { hasAlpha = value; };
	bool getAlpha() { return hasAlpha; };
	bool isTerrain();
	void setIsTerrain(bool YesNo);
	void setAlbedo(ID3D11ShaderResourceView *albedo);
	void setNormal(ID3D11ShaderResourceView *normal);
	void setAORoughMet(ID3D11ShaderResourceView *AORoughMet);
	void setTerrainMaterials(ID3D11ShaderResourceView *albedo1, ID3D11ShaderResourceView *normal1, ID3D11ShaderResourceView *AORoughMet1,
		ID3D11ShaderResourceView *albedo2, ID3D11ShaderResourceView *normal2, ID3D11ShaderResourceView *AORoughMet2,
		ID3D11ShaderResourceView *albedo3, ID3D11ShaderResourceView *normal3, ID3D11ShaderResourceView *AORoughMet3, ID3D11ShaderResourceView * ID_MAP);

	void bindMaterial();

	void update();


	int renderIndex = -1;

private:
	ID3D11ShaderResourceView * ID_MAP = nullptr;
	ID3D11ShaderResourceView * TerrainAlbedo_1 = nullptr;
	ID3D11ShaderResourceView * TerrainNormal_1 = nullptr;
	ID3D11ShaderResourceView * TerrainAORoughMet_1 = nullptr;
	ID3D11ShaderResourceView * TerrainAlbedo_2 = nullptr;
	ID3D11ShaderResourceView * TerrainNormal_2 = nullptr;
	ID3D11ShaderResourceView * TerrainAORoughMet_2 = nullptr;
	ID3D11ShaderResourceView * TerrainAlbedo_3 = nullptr;
	ID3D11ShaderResourceView * TerrainNormal_3 = nullptr;
	ID3D11ShaderResourceView * TerrainAORoughMet_3 = nullptr;
	bool terrain = false;

	bool hasAlpha = false;
	ID3D11ShaderResourceView * albedo = nullptr;
	ID3D11ShaderResourceView * normal = nullptr;
	ID3D11ShaderResourceView * AORoughMet = nullptr;
	ShaderProgram* shaderProgram = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
};

