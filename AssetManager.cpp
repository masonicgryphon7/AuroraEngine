#include "AssetManager.h"
#include "Console.h"


AssetManager::AssetManager()
{}

AssetManager::AssetManager(ID3D11Device * device, ID3D11DeviceContext * devContext)
{
	this->device = device;
	this->devContext = devContext;
}


AssetManager::~AssetManager()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}

	for (int i = 0; i < textures.size(); i++)
	{
		delete textures[i];
	}

	for (int i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}

	for (int i = 0; i < shaderPrograms.size(); i++)
	{
		delete shaderPrograms[i];
	}
}

void AssetManager::addTexture(std::string filePath)
{
	textures.push_back(new Texture(device, devContext, filePath));
	Console.success("Successfully added texture at: ", filePath);
}

void AssetManager::addMaterial(ShaderProgram * shaderProgram)
{
	materials.push_back(new Material(devContext, shaderProgram));
	Console.success("Successfully added material");
}

void AssetManager::addMesh(std::string filePath)
{
	meshes.push_back(new Mesh(filePath, device, devContext));

}

void AssetManager::addShaderProgram(INPUT_ELEMENT_DESCRIPTION description, std::string vertexShader, std::string hullShader, std::string domainShader, std::string geometryShader, std::string pixelShader, std::string computeShader)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> descArr;

	switch (description)
	{
	case INPUT_ELEMENT_DESCRIPTION::INPUT_ELEMENT_POS3UV2T3B3N3:

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

	shaderPrograms.push_back(new ShaderProgram(devContext, device, descArr, vertexShader, hullShader, domainShader, geometryShader, pixelShader, computeShader));
}

Texture * AssetManager::getTexture(int index)
{
	return textures.at(index);
}

Material * AssetManager::getMaterial(int index)
{
	return materials.at(index);
}

Mesh * AssetManager::getMesh(int index)
{
	return meshes.at(index);
}

ShaderProgram * AssetManager::getShaderProgram(int index)
{
	return shaderPrograms.at(index);
}
