#include "Mesh.h"

Mesh::Mesh()
{}

Mesh::Mesh(int vertCountData, std::vector<VERTEX_POS3UV2T3B3N3>* TerrainInfoVector, ID3D11Device * device, ID3D11DeviceContext * devContext)
{
	vertexCount = 0;
	gDeviceContext = devContext;
	CreateTerrainMeshData(vertCountData, TerrainInfoVector, device, devContext);
}

Mesh::Mesh(std::string filePath, ID3D11Device * device, ID3D11DeviceContext * devContext)
{
	vertexCount = 0;
	gDeviceContext = devContext;
	CreateMeshData(filePath, device, devContext);
}

Mesh::~Mesh()
{}


void Mesh::CreateMeshData(std::string fileName, ID3D11Device * device, ID3D11DeviceContext * devContext)
{
	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT2> texCoords;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<OBJFace> faces;
	std::vector<VERTEX_POS3UV2T3B3N3> vertex;


	std::ifstream objFile(fileName);
	std::string readString;
	while (!objFile.eof())
	{
		objFile >> readString;
		/*if (readString == "mtllib")
		{
		objFile >> readString;
		std::ifstream mtlFile(readString);
		while (!mtlFile.eof())
		{
		mtlFile >> readString;
		if (readString == "map_Kd")
		{
		this->isTextured = true;
		mtlFile >> readString;
		wchar_t* TextureFileName = new wchar_t[readString.size() + 1];
		HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
		if (FAILED(hr))
		{
		MessageBox(0, L"HEllo coinit failed", 0, 0);
		}
		for (int i = 0; i < readString.size() + 1; i++)
		{
		TextureFileName[i] = readString[i];
		}

		hr = DirectX::CreateWICTextureFromFile(dev, devCon,
		TextureFileName, nullptr, &textureResourceView);
		shaderResourceViews[0] = textureResourceView;
		delete[] TextureFileName;
		}
		if (readString == "bump")
		{

		mtlFile >> readString;

		wchar_t* TextureFileName = new wchar_t[readString.size() + 1];

		for (int i = 0; i < readString.size() + 1; i++)
		{
		TextureFileName[i] = readString[i];
		}

		HRESULT hr = DirectX::CreateWICTextureFromFile(dev, devCon,
		TextureFileName, nullptr, &textureResourceView);
		shaderResourceViews[1] = textureResourceView;
		this->hasNormal = true;
		delete[] TextureFileName;
		}
		if (readString == "Ka")
		{
		mtlFile >> KA.x;
		mtlFile >> KA.y;
		mtlFile >> KA.z;
		}
		if (readString == "Ks")
		{
		mtlFile >> KS.x;
		mtlFile >> KS.y;
		mtlFile >> KS.z;
		}
		if (readString == "Kd")
		{
		mtlFile >> KD.x;
		mtlFile >> KD.y;
		mtlFile >> KD.z;
		}
		}
		mtlFile.close();
		}*/
		if (readString == "v")
		{
			DirectX::XMFLOAT3 tempvec;
			objFile >> tempvec.x >> tempvec.y >> tempvec.z;
			positions.push_back(tempvec);
		}
		if (readString == "vn")
		{
			DirectX::XMFLOAT3 tempvec;
			objFile >> tempvec.x >> tempvec.y >> tempvec.z;
			normals.push_back(tempvec);
		}
		if (readString == "vt")
		{
			DirectX::XMFLOAT2 tempvec;
			objFile >> tempvec.x >> tempvec.y;
			tempvec.y = 1.0f - tempvec.y;
			texCoords.push_back(tempvec);
		}
		if (readString == "f")
		{
			OBJFace tempStruct;
			char tempChar;
			objFile >> tempStruct.v1 >> tempChar >> tempStruct.vt1 >> tempChar >> tempStruct.vn1 >>
				tempStruct.v2 >> tempChar >> tempStruct.vt2 >> tempChar >> tempStruct.vn2 >>
				tempStruct.v3 >> tempChar >> tempStruct.vt3 >> tempChar >> tempStruct.vn3;
			faces.push_back(tempStruct);
		}

	}
	objFile.close();

	//Pos, normal, uv
	for (int i = 0; i < faces.size() - 1; i++)
	{
		VERTEX_POS3UV2T3B3N3 vertex1;
		VERTEX_POS3UV2T3B3N3 vertex2;
		VERTEX_POS3UV2T3B3N3 vertex3;

		vertex1.position = positions[faces[i].v1 - 1];
		vertex1.normal = normals[faces[i].vn1 - 1];
		vertex1.uv = texCoords[faces[i].vt1 - 1];

		vertex2.position = positions[faces[i].v2 - 1];
		vertex2.normal = normals[faces[i].vn2 - 1];
		vertex2.uv = texCoords[faces[i].vt2 - 1];

		vertex3.position = positions[faces[i].v3 - 1];
		vertex3.normal = normals[faces[i].vn3 - 1];
		vertex3.uv = texCoords[faces[i].vt3 - 1];


		vertex.push_back(vertex1);
		vertex.push_back(vertex2);
		vertex.push_back(vertex3);

	}

	//Tangents and bitangents

	for (int i = 0; i < vertex.size() - 1; i += 3)
	{


		DirectX::XMFLOAT3 vec1 = subtract(vertex[i + 1].position, vertex[i].position);
		DirectX::XMFLOAT3 vec2 = subtract(vertex[i + 2].position, vertex[i].position);

		DirectX::XMFLOAT2 uVec1 = subtract(vertex[i + 1].uv, vertex[i].uv);
		DirectX::XMFLOAT2 uVec2 = subtract(vertex[i + 2].uv, vertex[i].uv);

		float denominator = (uVec1.x * uVec2.y) - (uVec1.y * uVec2.x);
		float someFloat = 1.0f / denominator;

		DirectX::XMFLOAT3 dVec1;
		DirectX::XMFLOAT3 dVec2;

		dVec1.x = vec1.x*uVec2.y;
		dVec1.y = vec1.y*uVec2.y;
		dVec1.z = vec1.z*uVec2.y;

		dVec2.x = vec2.x*uVec1.y;
		dVec2.y = vec2.y*uVec1.y;
		dVec2.z = vec2.z*uVec1.y;

		DirectX::XMFLOAT3 tan = subtract(dVec1, dVec2);
		tan.x = tan.x*someFloat;
		tan.y = tan.y*someFloat;
		tan.y = tan.z*someFloat;


		//tangent
		DirectX::XMVECTOR tangent = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&tan));
		DirectX::XMStoreFloat3(&vertex[i].tangent, tangent);
		DirectX::XMStoreFloat3(&vertex[i + 1].tangent, tangent);
		DirectX::XMStoreFloat3(&vertex[i + 2].tangent, tangent);

		DirectX::XMVECTOR bitangent = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&vertex[i].tangent), DirectX::XMLoadFloat3(&vertex[i].normal)));
		DirectX::XMStoreFloat3(&vertex[i].bitangent, bitangent);
		DirectX::XMStoreFloat3(&vertex[i + 1].bitangent, bitangent);
		DirectX::XMStoreFloat3(&vertex[i + 2].bitangent, bitangent);


	}

	//Create vertex buffer

	D3D11_BUFFER_DESC vertexBufferDesc;

	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	vertexBufferDesc.ByteWidth = sizeof(VERTEX_POS3UV2T3B3N3)  * vertex.size();
	D3D11_SUBRESOURCE_DATA vertexdata;
	ZeroMemory(&vertexdata, sizeof(D3D11_SUBRESOURCE_DATA));

	vertexdata.pSysMem = vertex.data();
	vertexdata.SysMemPitch = 0;
	vertexdata.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufferDesc, &vertexdata, &vertexBuffer);
	vertexSize = sizeof(VERTEX_POS3UV2T3B3N3);
	vertexCount = vertex.size();

}

void Mesh::bindMesh()
{
	UINT32 offset = 0;
	// specify which vertex buffer to use next.
	gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);

}


HRESULT Mesh::CreateTerrainMeshData(int vertCountData, std::vector<VERTEX_POS3UV2T3B3N3>* TerrainInfoVector, ID3D11Device * device, ID3D11DeviceContext * devContext)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;

	//ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_POS3UV2T3B3N3) * vertCountData;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = TerrainInfoVector->data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	if (FAILED(result))
	{
		return result;
	}

	vertexSize = sizeof(VERTEX_POS3UV2T3B3N3);
	vertexCount = TerrainInfoVector->size();
}

int Mesh::getVertexCount()
{
	return vertexCount;
}



DirectX::XMFLOAT3 Mesh::subtract(DirectX::XMFLOAT3 A, DirectX::XMFLOAT3 B)
{
	DirectX::XMFLOAT3 tempVec;

	tempVec.x = A.x - B.x;
	tempVec.y = A.y - B.y;
	tempVec.z = A.z - B.z;



	return tempVec;
}

DirectX::XMFLOAT2 Mesh::subtract(DirectX::XMFLOAT2 A, DirectX::XMFLOAT2 B)
{
	DirectX::XMFLOAT2 tempVec;

	tempVec.x = A.x - B.x;
	tempVec.y = A.y - B.y;



	return tempVec;
}