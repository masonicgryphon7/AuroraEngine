#include "ShaderProgram.h"

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 

ShaderProgram::ShaderProgram()
{}


ShaderProgram::~ShaderProgram()
{
	SAFE_RELEASE(gVertexLayout);
	SAFE_RELEASE(gVertexShader);
	SAFE_RELEASE(gHullShader);
	SAFE_RELEASE(gDomainShader);
	SAFE_RELEASE(gGeometryShader);
	SAFE_RELEASE(gPixelShader);
	SAFE_RELEASE(gComputeShader);
}

const std::string ShaderProgram::getShaderName()const {
	return this->shaderName;
}
const std::string ShaderProgram::getShaderPath()const {
	return this->shaderPath;
}

std::wstring ShaderProgram::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


HRESULT ShaderProgram::createVertexShader(ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, std::string filePath, SHADER_TYPE type)
{
	this->shaderPath = filePath;
	this->type = type;
	std::reverse(filePath.begin(), filePath.end());
	this->shaderName = filePath.substr(0, filePath.find("/", 0));
	std::reverse(this->shaderName.begin(), this->shaderName.end());
	this->shaderName = shaderName.substr(0, shaderName.find(".", 0));

	this->gDeviceContext = gDeviceContext;

	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;

	std::wstring stemp = s2ws(shaderPath);
	LPCWSTR sResult = stemp.c_str();



		ID3DBlob* pVS = nullptr;


		// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
		result = D3DCompileFromFile(
			sResult, // filename
			nullptr,		// optional macros
			nullptr,		// optional include files
			"VS_main",		// entry point
			"vs_5_0",		// shader model (target)
			D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
			0,				// IGNORE...DEPRECATED.
			&pVS,			// double pointer to ID3DBlob		
			&errorBlob		// pointer for Error Blob messages.
		);

		// compilation failed?
		if (FAILED(result))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				// release "reference" to errorBlob interface object
				errorBlob->Release();
			}
			if (pVS)
				pVS->Release();
			return result;
		}

		gDevice->CreateVertexShader(
			pVS->GetBufferPointer(),
			pVS->GetBufferSize(),
			nullptr,
			&gVertexShader
		);

		// create input layout (verified using vertex shader)
		// Press F1 in Visual Studio with the cursor over the datatype to jump
		// to the documentation online!
		// please read:
		// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx


		gDevice->CreateInputLayout(inputDesc.data(), inputDesc.size(), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);

		// we do not need anymore this COM object, so we release it.
		pVS->Release();
	



}

HRESULT ShaderProgram::createPixelShader(ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, std::string filePath, SHADER_TYPE type)
{
	this->shaderPath = filePath;
	this->type = type;

	std::reverse(filePath.begin(), filePath.end());
	this->shaderName = filePath.substr(0, filePath.find("/", 0));
	std::reverse(this->shaderName.begin(), this->shaderName.end());
	this->shaderName = shaderName.substr(0, shaderName.find(".", 0));

	this->gDeviceContext = gDeviceContext;

	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;

	std::wstring stemp = s2ws(shaderPath);
	LPCWSTR sResult = stemp.c_str();




		//create pixel shader
		ID3DBlob* pPS = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		result = D3DCompileFromFile(
			sResult, // filename
			nullptr,		// optional macros
			nullptr,		// optional include files
			"PS_main",		// entry point
			"ps_5_0",		// shader model (target)
			D3DCOMPILE_DEBUG,	// shader compile options
			0,				// effect compile options
			&pPS,			// double pointer to ID3DBlob		
			&errorBlob			// pointer for Error Blob messages.
		);

		// compilation failed?
		if (FAILED(result))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				// release "reference" to errorBlob interface object
				errorBlob->Release();
			}
			if (pPS)
				pPS->Release();
			return result;
		}

		gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);
		// we do not need anymore this COM object, so we release it.
		pPS->Release();

	


}

void ShaderProgram::ActivateShader()
{

	// specifying NULL or nullptr we are disabling that stage
	// in the pipeline
	switch (type)
	{
	case VERTEX_SHADER:
		// specify the topology to use when drawing
		// specify the IA Layout (how is data passed)
		gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
		gDeviceContext->IASetInputLayout(gVertexLayout);
		gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case HULL_SHADER:
		gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		break;
	case DOMAIN_SHADER:
		gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		break;
	case GEOMETRY_SHADER:
		gDeviceContext->GSSetShader(nullptr, nullptr, 0);
		break;
	case PIXEL_SHADER:
		gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
		break;
	case COMPUTE_SHADER:
		gDeviceContext->CSSetShader(nullptr, nullptr, 0);

		break;
	default:
		break;
	}

}
