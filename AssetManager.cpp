#include "AssetManager.h"
#include "Console.h"
#include <algorithm>
#include <iomanip>
#include "Scene.h"
#include "FullSkeleton.h"

std::vector<Texture*> cAssetManager::textures;
std::vector<AnimationClip*> cAssetManager::animationClips;

std::vector<Material*> cAssetManager::materials;
std::vector<Mesh*> cAssetManager::meshes;
std::vector<ShaderProgram*> cAssetManager::shaderPrograms;

ID3D11Device * cAssetManager::device;
ID3D11DeviceContext * cAssetManager::devContext;


cAssetManager::cAssetManager()
{}

cAssetManager::cAssetManager(ID3D11Device * device, ID3D11DeviceContext * devContext)
{
	this->device = device;
	this->devContext = devContext;
}

// Apparently this gets called multiple times.... why???
cAssetManager::~cAssetManager()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}

	meshes.clear();

	for (int i = 0; i < textures.size(); i++)
	{
		delete textures[i];
	}

	textures.clear();

	for (int i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}

	materials.clear();

	for (int i = 0; i < shaderPrograms.size(); i++)
	{
		delete shaderPrograms[i];
	}

	shaderPrograms.clear();
}

void cAssetManager::addTexture(std::string filePath)
{
	textures.push_back(new Texture(device, devContext, filePath));
	//Console.success("Successfully added texture at: ", filePath);
}

Texture * cAssetManager::AddTexture(std::string filePath)
{

	Texture* temp = nullptr;
	bool makingSureBool = false;

	for (auto& texture : textures)
	{
		if (texture->getPath() == filePath)
		{
			//Debug.Log("It actually contained the texture... LET'S BAIL");
			temp = texture;
			makingSureBool = true;
			break;
		}
	}

	if (temp == nullptr || !makingSureBool)
	{
		temp = new Texture(device, devContext, filePath);
		textures.push_back(temp);
	}

	return temp;
}

void cAssetManager::addMaterial(ShaderProgram * shaderProgram)
{
	materials.push_back(new Material(devContext, shaderProgram));
	//Console.success("Successfully added material");
}

Material * cAssetManager::AddMaterial(ShaderProgram * shaderProgram)
{
	Material* temp = nullptr;
	bool makingSureBool = false;

	std::string guid = GenerateGUIDAsString();

	for (auto& mat : materials)
	{
		if (!gScene.ContainsGUID(guid))
		{
			temp = mat;
			makingSureBool = true;
			//Debug.Log("Contains!!!");
			break;
		}
	}

	if (temp == nullptr || !makingSureBool)
	{
		temp = new Material(devContext, shaderProgram);
		materials.push_back(temp);
	}

	return temp;
}

void cAssetManager::addMesh(std::string filePath)
{
	meshes.push_back(new Mesh(filePath, device, devContext));
}


void cAssetManager::addMesh(int vertCountData, std::vector<VERTEX_POS3UV2T3B3N3>* TerrainInfoVector)
{
	meshes.push_back(new Mesh(vertCountData, TerrainInfoVector, device, devContext));
}

void cAssetManager::addMeshFromBinary(std::string filePath)
{
	meshes.push_back(new Mesh(filePath, device, devContext, true));
}

Mesh * cAssetManager::AddMesh(const std::string & filePath)
{
	Mesh* temp = nullptr;
	bool makingSureBool = false;
	for (auto& mesh : meshes)
	{
		if (mesh->getMeshPath() == filePath)
		{
			temp = mesh;
			makingSureBool = true;
			break;
		}
	}

	if (temp == nullptr || !makingSureBool)
	{
		temp = new Mesh(filePath, device, devContext);
		meshes.push_back(temp);
	}

	return temp;
}

void cAssetManager::addShaderProgram(INPUT_ELEMENT_DESCRIPTION description, std::string vertexShader, std::string hullShader, std::string domainShader, std::string geometryShader, std::string pixelShader, std::string computeShader)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> descArr;

	switch (description)
	{
	case INPUT_ELEMENT_DESCRIPTION::INPUT_ELEMENT_POS3UV2T3B3N3:
		{
			D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
				{
					"POSITION",		// "semantic" name in shader
					0,				// "semantic" index (not used)
					DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
					0,							 // input slot
					0,							 // offset of first element
					D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
					0							 // used for INSTANCING (ignore)
				},
				{
					"UV",
					0,				// same slot as previous (same vertexBuffer)
					DXGI_FORMAT_R32G32_FLOAT,
					0,
					12,							// offset of FIRST element (after POSITION)
					D3D11_INPUT_PER_VERTEX_DATA,
					0
				},
				{
					"TANGENT",		// "semantic" name in shader
					0,				// "semantic" index (not used)
					DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
					0,							 // input slot
					20,							 // offset of first element
					D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
					0							 // used for INSTANCING (ignore)
				},{
					"BITANGENT",		// "semantic" name in shader
					0,				// "semantic" index (not used)
					DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
					0,							 // input slot
					32,							 // offset of first element
					D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
					0							 // used for INSTANCING (ignore)
				},{
					"NORMAL",		// "semantic" name in shader
					0,				// "semantic" index (not used)
					DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
					0,							 // input slot
					44,							 // offset of first element
					D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
					0							 // used for INSTANCING (ignore)
				}
			};
			descArr.push_back(inputDesc[0]);
			descArr.push_back(inputDesc[1]);
			descArr.push_back(inputDesc[2]);
			descArr.push_back(inputDesc[3]);
			descArr.push_back(inputDesc[4]);
			break;
		}
	case INPUT_ELEMENT_DESCRIPTION::INPUT_ELEMENT_POS3UV2T3B3N3JNT4WT4:

		D3D11_INPUT_ELEMENT_DESC inputDesc1[] = {
			{
				"POSITION",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				0,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},
			{
				"UV",
				0,				// same slot as previous (same vertexBuffer)
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				12,							// offset of FIRST element (after POSITION)
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"TANGENT",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				20,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},{
				"BITANGENT",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				32,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},{
				"NORMAL",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				44,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},{
				"JOINTINDEX",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				56,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			},{
				"WEIGHT",		// "semantic" name in shader
				0,				// "semantic" index (not used)
				DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
				0,							 // input slot
				72,							 // offset of first element
				D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
				0							 // used for INSTANCING (ignore)
			}
		};
		descArr.push_back(inputDesc1[0]);
		descArr.push_back(inputDesc1[1]);
		descArr.push_back(inputDesc1[2]);
		descArr.push_back(inputDesc1[3]);
		descArr.push_back(inputDesc1[4]);
		descArr.push_back(inputDesc1[5]);
		descArr.push_back(inputDesc1[6]);
		break;
	}

	shaderPrograms.push_back(new ShaderProgram(devContext, device, descArr, vertexShader, hullShader, domainShader, geometryShader, pixelShader, computeShader));
}

void cAssetManager::addAnimationClipFromBinary(const std::string & filePath)
{
	AnimationClip* temp = nullptr;
	bool makingSureBool = false;
	for (auto& clip : animationClips)
	{
		if (clip->getClipPath() == filePath)
		{
			temp = clip;
			makingSureBool = true;
			break;
		}
	}

	if (temp == nullptr || !makingSureBool)
	{
		temp = new AnimationClip();
		temp->createClipFromBinary(filePath);
		animationClips.push_back(temp);
	}

}

void cAssetManager::Start(ID3D11Device * device, ID3D11DeviceContext * devContext)
{
	this->device = device;
	this->devContext = devContext;
}

void cAssetManager::addSkeletonFromBinary(const std::string & filePath)
{
}

MyLibrary::SkeletonFromFile * cAssetManager::getSkeleton(const std::string & filePath)
{
	bool hasFound = false;
	MyLibrary::SkeletonFromFile* skeleton = nullptr;
	FullSkeleton *temp;
	for (auto& skeleton : animationClips)
	{
		if (skeleton->getClipPath() == filePath)
		{
			Console.success("Found texture at, returning its value from ", filePath);
			hasFound = true;
			skeleton = skeleton;
		}
	}

	if (!hasFound)
	{
		temp = new FullSkeleton();
		temp->GetSkeletonFromBinary(filePath);
		//skeletons.push_back(temp);
		//Ta bort deleten
		delete temp;
	}

	return skeleton;
}

AnimationClip * cAssetManager::getAnimationclip(const std::string & filePath)
{
	bool hasFound = false;
	AnimationClip* clip = nullptr;

	for (auto& clip : animationClips)
	{
		if (clip->getClipPath() == filePath)
		{
			Console.success("Found texture at, returning its value from ", filePath);
			hasFound = true;
			clip = clip;
		}
	}

	if (!hasFound)
	{
		clip = new AnimationClip();
		clip->createClipFromBinary(filePath);
		animationClips.push_back(clip);
	}

	return clip;
}

Texture * cAssetManager::getTexture(const std::string & path)
{
	bool hasFound = false;
	Texture* tex = nullptr;

	for (auto& t : textures)
	{
		if (t->getPath() == path)
		{
			Console.success("Found texture at, returning its value from ", path);
			hasFound = true;
			tex = t;
		}
	}

	if (!hasFound)
	{
		tex = new Texture(device, devContext, path);
		textures.push_back(tex);
	}

	return tex;
}

Texture * cAssetManager::getTexture(int index)
{
	return textures.at(index);
}

Material * cAssetManager::getMaterial(int index)
{
	return materials.at(index);
}

Mesh * cAssetManager::getMesh(int index)
{
	return meshes.at(index);
}

#include "Debug.h"
Mesh * cAssetManager::getMesh(const std::string & name)
{
	Mesh* temp = nullptr;

	for (auto m : meshes)
	{
		if (m->getMeshName() == name)
		{
			temp = m;
			// can return here but might add some stuff?
			break;
		}
	}

	return temp;
}

ShaderProgram * cAssetManager::getShaderProgram(int index)
{
	return shaderPrograms.at(index);
}

unsigned int cAssetManager::GenerateGUID()
{
	hash<std::string> hasher;
	return (unsigned int)hasher(GenerateGUIDAsString());
}

std::string cAssetManager::GenerateGUIDAsString()
{
	std::string guidString = "NULL";
	GUID guid;
	HRESULT hr = CoCreateGuid(&guid);
	// setw sets the field width of the current output operation...
	// https://stackoverflow.com/questions/556997/how-can-i-create-a-guid-in-mfc
	// https://stackoverflow.com/questions/1672677/print-a-guid-variable
	if (SUCCEEDED(hr))
	{
		std::stringstream stream;
		stream << hex << uppercase << setw(8) << setfill('0') << guid.Data1 <<
			"-" << setw(4) << setfill('0') << guid.Data2 << "-" << setw(4) << setfill('0') << guid.Data3 << "-";

		for (unsigned int i = 0; i < sizeof(guid.Data4); ++i)
		{
			if (i == 2)
				stream << "-";

			stream << hex << setw(2) << setfill('0') << int(guid.Data4[i]);
		}
		guidString = stream.str();
	}

	return guidString;
}

std::string cAssetManager::GUIDToString(unsigned int guid)
{
	return std::to_string(guid);
}

unsigned int cAssetManager::GUIDToUnsignedInt(const std::string & guid)
{
	std::stringstream stream(guid);
	unsigned int guidSize;
	stream >> guidSize;

	return guidSize;
}
