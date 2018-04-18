#include "Texture.h"



Texture::Texture()
{}

Texture::Texture(ID3D11Device * device, ID3D11DeviceContext * devContext, std::string filePath)
{
	this->pathToTexture = filePath;
	createTextureData(device, devContext, filePath);
}


Texture::~Texture()
{
	textureView->Release();
}

void Texture::createTextureData(ID3D11Device * device, ID3D11DeviceContext * devContext, std::string filePath)
{
	std::wstring widestr = std::wstring(filePath.begin(), filePath.end());
	const wchar_t* widecstr = widestr.c_str();

	HRESULT hr = DirectX::CreateWICTextureFromFile(device, devContext,
		widecstr, nullptr, &textureView);

	int t = 0;

}
