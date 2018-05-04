#pragma once
#include <vector>
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "INPUT_ELEMENT_DESCRIPTION.h"
#include "PRIMITIVE_GEOMETRY.h"
#include "AnimationClip.h"

static class cAssetManager
{
public:
	cAssetManager();
	cAssetManager(ID3D11Device * device, ID3D11DeviceContext * devContext);
	~cAssetManager();

	void addTexture(std::string filePath);
	Texture* AddTexture(std::string filePath);
	void addMaterial(ShaderProgram * shaderProgram);
	Material* AddMaterial(ShaderProgram* shaderProgram);
	void addMesh(std::string filePath);
	void addMesh(int vertCountData, std::vector<VERTEX_POS3UV2T3B3N3>* TerrainInfoVector);
	Mesh* AddMesh(const std::string& filePath);
	void addMeshFromBinary(std::string filePath);
	void addShaderProgram(INPUT_ELEMENT_DESCRIPTION description, std::string vertexShader, std::string hullShader, std::string domainShader, std::string geometryShader, std::string pixelShader, std::string computeShader);
	void addAnimationClipFromBinary(const std::string& filePath);
	void Start(ID3D11Device * device, ID3D11DeviceContext * devContext);

	AnimationClip* getAnimationclip(const std::string& filePath);
	Texture* getTexture(const std::string &path);
	Texture* getTexture(int index);
	Material* getMaterial(int index);
	Mesh* getMesh(int index);
	Mesh* getMesh(const std::string& name);
	ShaderProgram* getShaderProgram(int index);

	// -- GUID -- //
	static unsigned int GenerateGUID();
	static std::string GenerateGUIDAsString();
	static std::string GUIDToString(unsigned int guid);
	static unsigned int GUIDToUnsignedInt(const std::string& guid);

private:
	static std::vector<Texture*> textures;
	static std::vector<AnimationClip*> animationClips;

	static std::vector<Material*> materials;
	static std::vector<Mesh*> meshes;
	static std::vector<ShaderProgram*> shaderPrograms;

	static ID3D11Device * device;
	static ID3D11DeviceContext * devContext;
} AssetManager;

