#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(int vertCountData, std::string terrainName, std::vector<VERTEX_POS3UV2T3B3N3> TerrainInfoVector, ID3D11Device * device, ID3D11DeviceContext * devContext, ShaderProgram* vertexShader)
{
	this->vertexShader = vertexShader;
	vertexCount = 0;
	gDeviceContext = devContext;
	CreateTerrainMeshData(vertCountData, TerrainInfoVector, device, devContext);
	this->meshName = terrainName;
	this->meshPath = terrainName;
}

Mesh::Mesh(std::string filePath, ID3D11Device * device, ID3D11DeviceContext * devContext, ShaderProgram* vertexShader)
{
	this->vertexShader = vertexShader;

	vertexCount = 0;
	gDeviceContext = devContext;
	CreateMeshData(filePath, device, devContext);

	this->meshPath = filePath;

	std::reverse(filePath.begin(), filePath.end());
	this->meshName = filePath.substr(0, filePath.find("/", 0));
	std::reverse(this->meshName.begin(), this->meshName.end());
	this->meshName = meshName.substr(0, meshName.find(".", 0));
}

Mesh::Mesh(std::string filePath, ID3D11Device * device, ID3D11DeviceContext * devContext, bool isBinary, bool isAnimated, ShaderProgram* vertexShader)
{
	this->vertexShader = vertexShader;
	this->meshPath = filePath;

	std::reverse(filePath.begin(), filePath.end());
	this->meshName = filePath.substr(0, filePath.find("/", 0));
	std::reverse(this->meshName.begin(), this->meshName.end());
	this->meshName = meshName.substr(0, meshName.find(".", 0));

	gDeviceContext = devContext;
	if (!isAnimated) {
		createMeshFromBinary(meshPath, device);
		vertexSize = sizeof(VERTEX_POS3UV2T3B3N3);
	}
	else
	{
		createAnimatedMeshFromBinary(meshPath, device);
		vertexSize = sizeof(VERTEX_POS3UV2T3B3N3JNT4WT4);
	}
}

Mesh::~Mesh()
{
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}


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


		vertexPositions.push_back(DirectX::XMLoadFloat3(&vertex1.position));
		vertexPositions.push_back(DirectX::XMLoadFloat3(&vertex2.position));
		vertexPositions.push_back(DirectX::XMLoadFloat3(&vertex3.position));


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
		tan.z = tan.z*someFloat;


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

	//VERTEX_POS3UV2T3B3N3 *vertex;

	//std::vector<VERTEX_POS3UV2T3B3N3*> vertex;

	//vertex[i].position.pushback.positionfromfile;


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
	vertexShader->ActivateShader();
	gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
}

const std::string Mesh::getMeshName() const
{
	return this->meshName;
}

const std::string Mesh::getMeshPath() const
{
	return this->meshPath;
}


HRESULT Mesh::CreateTerrainMeshData(int vertCountData, std::vector<VERTEX_POS3UV2T3B3N3> TerrainInfoVector, ID3D11Device * device, ID3D11DeviceContext * devContext)
{
	for (int i = 0; i < TerrainInfoVector.size(); i++)
	{
		vertexPositions.push_back(DirectX::XMLoadFloat3(&TerrainInfoVector[i].position));
		int gg = 0;
	}
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
	vertexData.pSysMem = TerrainInfoVector.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	if (FAILED(result))
	{
		return result;
	}

	vertexSize = sizeof(VERTEX_POS3UV2T3B3N3);
	vertexCount = TerrainInfoVector.size();
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

void Mesh::createMeshFromBinary(std::string fileName, ID3D11Device * device)
{
	//std::vector<DirectX::XMFLOAT3> positions;
	//std::vector<DirectX::XMFLOAT2> texCoords;
	//std::vector<DirectX::XMFLOAT3> normals;
	MyLibrary::Loadera myLoader;
	MyLibrary::MeshFromFile myMesh;
	VERTEX_POS3UV2T3B3N3 vertex;
	std::vector<VERTEX_POS3UV2T3B3N3> vertices;
	myLoader.readMeshFile(fileName, &myMesh);

	for (int i = 0; i < myMesh.mesh_nrOfVertices; i++)
	{
		vertex.position.x = myMesh.mesh_vertices[i].vertex_position[0];
		vertex.position.y = myMesh.mesh_vertices[i].vertex_position[1];
		vertex.position.z = myMesh.mesh_vertices[i].vertex_position[2];
		vertexPositions.push_back(DirectX::XMVectorSet(vertex.position.x, vertex.position.y, vertex.position.z, 0));

		vertex.normal.x = myMesh.mesh_vertices[i].vertex_normal[0];
		vertex.normal.y = myMesh.mesh_vertices[i].vertex_normal[1];
		vertex.normal.z = myMesh.mesh_vertices[i].vertex_normal[2];
		DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vertex.normal)));

		vertex.uv.x = myMesh.mesh_vertices[i].vertex_UVCoord[0];
		vertex.uv.y = myMesh.mesh_vertices[i].vertex_UVCoord[1];

		vertex.tangent.x = myMesh.mesh_vertices[i].vertex_tangent[0];
		vertex.tangent.y = myMesh.mesh_vertices[i].vertex_tangent[1];
		vertex.tangent.z = myMesh.mesh_vertices[i].vertex_tangent[2];
		DirectX::XMStoreFloat3(&vertex.tangent, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vertex.tangent)));

		vertex.bitangent.x = myMesh.mesh_vertices[i].vertex_biTangent[0];
		vertex.bitangent.y = myMesh.mesh_vertices[i].vertex_biTangent[1];
		vertex.bitangent.z = myMesh.mesh_vertices[i].vertex_biTangent[2];
		DirectX::XMStoreFloat3(&vertex.bitangent, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vertex.bitangent)));

		vertices.push_back(vertex);
	}
	

	D3D11_BUFFER_DESC vertexBufferDesc;

	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	vertexBufferDesc.ByteWidth = sizeof(vertices[0])  * vertices.size();
	D3D11_SUBRESOURCE_DATA vertexdata;
	ZeroMemory(&vertexdata, sizeof(D3D11_SUBRESOURCE_DATA));

	vertexdata.pSysMem = vertices.data();
	vertexdata.SysMemPitch = 0;
	vertexdata.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufferDesc, &vertexdata, &vertexBuffer);
	vertexSize = sizeof(VERTEX_POS3UV2T3B3N3);
	vertexCount = vertices.size();
}



void Mesh::createAnimatedMeshFromBinary(std::string fileName, ID3D11Device * device)
{
	//std::vector<DirectX::XMFLOAT3> positions;
	//std::vector<DirectX::XMFLOAT2> texCoords;
	//std::vector<DirectX::XMFLOAT3> normals;
	MyLibrary::Loadera myLoader;
	MyLibrary::AnimatedMeshFromFile myMesh;
	VERTEX_POS3UV2T3B3N3JNT4WT4 vertex;
	std::vector<VERTEX_POS3UV2T3B3N3JNT4WT4> vertices;
	myLoader.readAnimatedMeshFile(fileName, &myMesh);
	int gg = 0;
	for (int i = 0; i <   myMesh.mesh_nrOfVertices; i++)
	{
		vertex.position.x =  myMesh.mesh_vertices[i].vertex_position[0];
		vertex.position.y =  myMesh.mesh_vertices[i].vertex_position[1];
		vertex.position.z =  myMesh.mesh_vertices[i].vertex_position[2];
		vertexPositions.push_back(DirectX::XMVectorSet(vertex.position.x, vertex.position.y, vertex.position.z, 0));

		vertex.normal.x =  myMesh.mesh_vertices[i].vertex_normal[0];
		vertex.normal.y =  myMesh.mesh_vertices[i].vertex_normal[1];
		vertex.normal.z =  myMesh.mesh_vertices[i].vertex_normal[2];
		DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vertex.normal)));

		vertex.uv.x =  myMesh.mesh_vertices[i].vertex_UVCoord[0];
		vertex.uv.y =  myMesh.mesh_vertices[i].vertex_UVCoord[1];

		vertex.tangent.x =  myMesh.mesh_vertices[i].vertex_tangent[0];
		vertex.tangent.y =  myMesh.mesh_vertices[i].vertex_tangent[1];
		vertex.tangent.z =  myMesh.mesh_vertices[i].vertex_tangent[2];
		DirectX::XMStoreFloat3(&vertex.tangent, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vertex.tangent)));
		DirectX::XMStoreFloat3(&vertex.tangent, DirectX::XMVectorSet(0,0,0,0));


		vertex.bitangent.x =  myMesh.mesh_vertices[i].vertex_biTangent[0];
		vertex.bitangent.y =  myMesh.mesh_vertices[i].vertex_biTangent[1];
		vertex.bitangent.z =  myMesh.mesh_vertices[i].vertex_biTangent[2];
		DirectX::XMStoreFloat3(&vertex.bitangent, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vertex.bitangent)));
		DirectX::XMStoreFloat3(&vertex.bitangent, DirectX::XMVectorSet(0, 0, 0, 0));

		vertex.jointIndex.x =  myMesh.mesh_vertices[i].influencing_joint[0]-1;
		vertex.jointIndex.y =  myMesh.mesh_vertices[i].influencing_joint[1]-1;
		vertex.jointIndex.z =  myMesh.mesh_vertices[i].influencing_joint[2]-1;
		vertex.jointIndex.w =  myMesh.mesh_vertices[i].influencing_joint[3]-1;

		vertex.jointWeight.x =  myMesh.mesh_vertices[i].joint_weights[0];
		vertex.jointWeight.y =  myMesh.mesh_vertices[i].joint_weights[1];
		vertex.jointWeight.z =  myMesh.mesh_vertices[i].joint_weights[2];
		vertex.jointWeight.w =  myMesh.mesh_vertices[i].joint_weights[3];

		vertices.push_back(vertex);
	}

	gg = 0;
	D3D11_BUFFER_DESC vertexBufferDesc;

	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	vertexBufferDesc.ByteWidth = sizeof(VERTEX_POS3UV2T3B3N3JNT4WT4)  * vertices.size();
	D3D11_SUBRESOURCE_DATA vertexdata;
	ZeroMemory(&vertexdata, sizeof(D3D11_SUBRESOURCE_DATA));

	vertexdata.pSysMem = vertices.data();
	vertexdata.SysMemPitch = 0;
	vertexdata.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufferDesc, &vertexdata, &vertexBuffer);
	vertexSize = sizeof(VERTEX_POS3UV2T3B3N3JNT4WT4);
	vertexCount = vertices.size();
}
