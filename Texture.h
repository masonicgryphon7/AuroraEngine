#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include "DirectXTK-master\Inc\WICTextureLoader.h"

class Texture
{
public:
	Texture();
	Texture(ID3D11Device * device, ID3D11DeviceContext * devContext, std::string filePath);

	~Texture();

	void createTextureData(ID3D11Device * device, ID3D11DeviceContext * devContext, std::string filePath);

	ID3D11ShaderResourceView * getTexture() { return textureView; }
	const std::string getPath()  const { return pathToTexture; }
	const std::string getName()  const { return textureName; }

private:
	ID3D11ShaderResourceView * textureView = nullptr;
	std::string pathToTexture;
	std::string textureName;

};

