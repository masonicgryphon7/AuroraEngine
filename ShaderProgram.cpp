#include "ShaderProgram.h"

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 

ShaderProgram::ShaderProgram()
{}

ShaderProgram::ShaderProgram(ID3D11DeviceContext * gDeviceContext, ID3D11Device * gDevice, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, std::string vertexShader, std::string hullShader, std::string domainShader, std::string geometryShader, std::string pixelShader, std::string computeShader)
{
	CreateShaderData(gDeviceContext, gDevice, inputDesc, vertexShader, hullShader, domainShader, geometryShader, pixelShader, computeShader);
}

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


HRESULT ShaderProgram::CreateShaderData(ID3D11DeviceContext* gDeviceContext, ID3D11Device* gDevice, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, std::string vertexShader, std::string hullShader, std::string domainShader, std::string geometryShader, std::string pixelShader, std::string computeShader)
{
	this->gDeviceContext = gDeviceContext;

	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;

	std::wstring stemp = s2ws(vertexShader);
	LPCWSTR sResult = stemp.c_str();


	if (vertexShader != "")
	{
		ID3DBlob* pVS = nullptr;

		stemp = s2ws(vertexShader);
		sResult = stemp.c_str();

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

	if (pixelShader != "")
	{
		//create pixel shader
		ID3DBlob* pPS = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		stemp = s2ws(pixelShader);
		sResult = stemp.c_str();
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


}

void ShaderProgram::ActivateShader()
{

	// specifying NULL or nullptr we are disabling that stage
	// in the pipeline
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	// specify the topology to use when drawing
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// specify the IA Layout (how is data passed)
	gDeviceContext->IASetInputLayout(gVertexLayout);
}
