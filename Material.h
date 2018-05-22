#pragma once
#include "ShaderProgram.h"
#include <d3d11.h>
#include <d3dcompiler.h>
class Material
{
public:
	Material();
	Material(std::string name, ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, ShaderProgram* pixelShader);
	~Material();

	void setAlpha(bool value) { hasAlpha = value; };
	bool getAlpha() { return hasAlpha; };
	bool isTerrain();
	void setIsTerrain(bool YesNo);
	void setAlbedo(ID3D11ShaderResourceView *albedo);
	void setNormal(ID3D11ShaderResourceView *normal);
	void setAORoughMet(ID3D11ShaderResourceView *AORoughMet);
	void setTeamIdMap(ID3D11ShaderResourceView *teamIdMap);
	void setTerrainMaterials(ID3D11ShaderResourceView *albedo1, ID3D11ShaderResourceView *normal1, ID3D11ShaderResourceView *AORoughMet1,
		ID3D11ShaderResourceView *albedo2, ID3D11ShaderResourceView *normal2, ID3D11ShaderResourceView *AORoughMet2,
		ID3D11ShaderResourceView *albedo3, ID3D11ShaderResourceView *normal3, ID3D11ShaderResourceView *AORoughMet3,
		ID3D11ShaderResourceView *albedo4, ID3D11ShaderResourceView *normal4, ID3D11ShaderResourceView *AORoughMet4, ID3D11ShaderResourceView * ID_MAP);

	void bindMaterial();

	void update();
	void setXTile(float x) { xTile = x; };
	void setYTile(float y) { yTile = y; };
	float getXTile() { return xTile; };
	float getYTile() { return yTile; };
	const std::string getMaterialName() const;


	int renderIndex = -1;

	ShaderProgram* pixelShader = nullptr;
private:
	std::string materialName;

	ID3D11SamplerState * m_sampleState = nullptr;
	ID3D11SamplerState * m_sampleStateIDMAP = nullptr;

	float xTile = 1;
	float yTile = 1;

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
	ID3D11ShaderResourceView * TerrainAlbedo_4 = nullptr;
	ID3D11ShaderResourceView * TerrainNormal_4 = nullptr;
	ID3D11ShaderResourceView * TerrainAORoughMet_4 = nullptr;
	bool terrain = false;

	bool hasAlpha = false;
	ID3D11ShaderResourceView * albedo = nullptr;
	ID3D11ShaderResourceView * normal = nullptr;
	ID3D11ShaderResourceView * AORoughMet = nullptr;
	ID3D11ShaderResourceView * teamIdMap = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11Device* gDevice = nullptr;

	void createSamplerState();
	void createSamplerStateForIDMap();
};

