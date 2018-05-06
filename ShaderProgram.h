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

enum SHADER_TYPE {
	VERTEX_SHADER, HULL_SHADER,DOMAIN_SHADER,GEOMETRY_SHADER,PIXEL_SHADER,COMPUTE_SHADER
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	HRESULT createVertexShader(ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, std::string filePath, SHADER_TYPE type);
	HRESULT createPixelShader(ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, std::string filePath, SHADER_TYPE type);


	const std::string getShaderName() const;
	const std::string getShaderPath() const;
	void ActivateShader();
private:
	std::string shaderName, shaderPath;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	SHADER_TYPE type;

	ID3D11InputLayout* gVertexLayout = nullptr;
	ID3D11VertexShader* gVertexShader = nullptr;
	ID3D11HullShader* gHullShader = nullptr;
	ID3D11DomainShader* gDomainShader = nullptr;
	ID3D11GeometryShader* gGeometryShader = nullptr;
	ID3D11PixelShader* gPixelShader = nullptr;
	ID3D11ComputeShader* gComputeShader = nullptr;


	std::wstring s2ws(const std::string& s);

};

