#pragma once
// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))



#include <windows.h>
#include <string>
#include <fstream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <wrl.h>


class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, std::string vertexShader, std::string hullShader, std::string domainShader, std::string geometryShader, std::string pixelShader, std::string computeShader);

	~ShaderProgram();

	void ActivateShader();
private:
	HRESULT CreateShaderData(ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, std::string vertexShader, std::string hullShader, std::string domainShader, std::string geometryShader, std::string pixelShader, std::string computeShader);
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11InputLayout* gVertexLayout = nullptr;
	ID3D11VertexShader* gVertexShader = nullptr;
	ID3D11HullShader* gHullShader = nullptr;
	ID3D11DomainShader* gDomainShader = nullptr;
	ID3D11GeometryShader* gGeometryShader = nullptr;
	ID3D11PixelShader* gPixelShader = nullptr;
	ID3D11ComputeShader* gComputeShader = nullptr;


	std::wstring s2ws(const std::string& s);

};

