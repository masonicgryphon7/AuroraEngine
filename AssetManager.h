#pragma once
#include <vector>
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "PRIMITIVE_GEOMETRY.h"
#include "AnimationClip.h"
#include "Skeleton.h"

enum class INPUT_ELEMENT_DESCRIPTION
{
	INPUT_ELEMENT_POS3UV2T3B3N3, INPUT_ELEMENT_POS3UV2T3B3N3JNT4WT4,
};

static class cAssetManager
{
public:
	cAssetManager();
	cAssetManager(ID3D11Device * device, ID3D11DeviceContext * devContext);
	~cAssetManager();

	void addTexture(std::string filePath);
	Texture* AddTexture(std::string filePath);
	Material* AddMaterial(std::string name, ShaderProgram* shaderProgram);
	void addMesh(std::string filePath, ShaderProgram* vertexShader);
	void addMesh(int vertCountData, std::string terrainName, std::vector<VERTEX_POS3UV2T3B3N3> TerrainInfoVector, ShaderProgram* vertexShader);
	Mesh* AddMesh(const std::string& filePath, ShaderProgram* vertexShader);
	Mesh* AddMesh(int vertCountData, std::string terrainName, std::vector<VERTEX_POS3UV2T3B3N3> TerrainInfoVector, ShaderProgram* vertexShader);
	void addMeshFromBinary(std::string filePath, ShaderProgram* vertexShader);
	void addAnimatedMeshFromBinary(std::string filePath, ShaderProgram* vertexShader);
	void addShaderProgram(INPUT_ELEMENT_DESCRIPTION description,  std::string filePath, SHADER_TYPE type);
	void addShaderProgram(std::string filePath, SHADER_TYPE type);
	void addAnimationClipFromBinary(Skeleton* skeleton, const std::string& filePath);
	void Start(ID3D11Device * device, ID3D11DeviceContext * devContext);
	void addSkeletonFromBinary(const std::string &filePath);
	Skeleton* getSkeleton(const std::string& filePath);
	AnimationClip* getAnimationclip(Skeleton* skeleton, const std::string& filePath);
	Texture* getTexture(const std::string &path);
	Texture* getTexture(int index);
	Material* getMaterial(int index);
	Material* getMaterial(const std::string& name);
	Mesh* getMesh(int index);
	Mesh* getMesh(const std::string& name);
	ShaderProgram* getShaderProgram(int index);
	ShaderProgram* getShaderProgram(const std::string& filePath);
	ID3D11Device* getD3D11Device() { return device; };


	// -- GUID -- //
	static unsigned int GenerateGUID();
	static std::string GenerateGUIDAsString();
	static std::string GUIDToString(unsigned int guid);
	static unsigned int GUIDToUnsignedInt(const std::string& guid);

	std::vector<Texture*> AllTextures();

private:
	static std::vector<Texture*> textures;
	static std::vector<AnimationClip*> animationClips;
	static std::vector<Skeleton*> skeletons;
	static std::vector<Material*> materials;
	static std::vector<Mesh*> meshes;
	static std::vector<ShaderProgram*> shaderPrograms;

	static ID3D11Device * device;
	static ID3D11DeviceContext * devContext;
} AssetManager;

