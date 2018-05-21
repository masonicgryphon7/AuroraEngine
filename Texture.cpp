#include "Texture.h"



Texture::Texture()
{
}

Texture::Texture(ID3D11Device * device, ID3D11DeviceContext * devContext, std::string filePath)
{
	this->pathToTexture = filePath;

	std::reverse(filePath.begin(), filePath.end());
	this->textureName = filePath.substr(0, filePath.find("/", 0));
	std::reverse(this->textureName.begin(), this->textureName.end());
	this->textureName = textureName.substr(0, textureName.find(".", 0));

	createTextureData(device, devContext, pathToTexture);
}


Texture::~Texture()
{
	if (textureView != nullptr)
		textureView->Release();

	textureView = nullptr;
}

void Texture::createTextureData(ID3D11Device * device, ID3D11DeviceContext * devContext, std::string filePath)
{
	std::wstring widestr = std::wstring(filePath.begin(), filePath.end());
	const wchar_t* widecstr = widestr.c_str();

	HRESULT hr = DirectX::CreateWICTextureFromFile(device, devContext,
		widecstr, nullptr, &textureView);

	int t = 0;

}
