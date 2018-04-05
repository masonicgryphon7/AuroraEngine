#pragma once
#include <vector>
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "INPUT_ELEMENT_DESCRIPTION.h"

class AssetManager
{
public:
	AssetManager();
	AssetManager(ID3D11Device * device, ID3D11DeviceContext * devContext);
	~AssetManager();

	void addTexture(std::string filePath);
	void addMaterial(ShaderProgram * shaderProgram);
	void addMesh(std::string filePath);
	void addShaderProgram(INPUT_ELEMENT_DESCRIPTION description, std::string vertexShader, std::string hullShader, std::string domainShader, std::string geometryShader, std::string pixelShader, std::string computeShader);

	Texture* getTexture(int index);
	Material* getMaterial(int index);
	Mesh* getMesh(int index);
	ShaderProgram* getShaderProgram(int index);

private:
	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	std::vector<Mesh*> meshes;
	std::vector<ShaderProgram*> shaderPrograms;

	ID3D11Device * device;
	ID3D11DeviceContext * devContext;
};

