#pragma once
// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))


#include <windows.h>
#include <string>
#include "PRIMITIVE_GEOMETRY.h"
#include <vector>
#include <iostream>
#include <algorithm>    // std::unique, std::distance
#include <DirectXMath.h>
#include "DirectXTK-master\Inc\DDSTextureLoader.h"
#include"DirectXTK-master\Inc\Model.h"
#include <fstream>
#include "ShaderProgram.h"
#include "formatImporter.h"
#pragma comment(lib, "myLibrary.lib")

struct OBJFace
{
	int v1, vt1, vn1;
	int v2, vt2, vn2;
	int v3, vt3, vn3;
};

class Mesh
{
public:
	Mesh();
	Mesh(int vertCountData, std::string terrainName, std::vector<VERTEX_POS3UV2T3B3N3> TerrainInfoVector, ID3D11Device * device, ID3D11DeviceContext * devContext, ShaderProgram* vertexShader);
	Mesh(std::string filePath, ID3D11Device * device, ID3D11DeviceContext * devContext, ShaderProgram* vertexShader);
	Mesh(std::string filePath, ID3D11Device * device, ID3D11DeviceContext * devContext, bool isBinary, bool isAnimated, ShaderProgram* vertexShader);

	~Mesh();

	std::vector<DirectX::XMVECTOR>* getVertexPositions() { return &vertexPositions; };
	int getVertexCount();
	void bindMesh();

	const std::string getMeshName() const;
	const std::string getMeshPath() const;

	int renderIndex = -1;
private:
	ShaderProgram * vertexShader;
	int vertexCount;
	UINT32 vertexSize;
	ID3D11Buffer *vertexBuffer;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	std::vector<DirectX::XMVECTOR> vertexPositions;


	std::string meshName, meshPath;
	void createAnimatedMeshFromBinary(std::string fileName, ID3D11Device * device);

	void createMeshFromBinary(std::string fileName, ID3D11Device * device);
	HRESULT CreateTerrainMeshData(int vertCountData, std::vector<VERTEX_POS3UV2T3B3N3> TerrainInfoVector, ID3D11Device *device, ID3D11DeviceContext *devContext);
	void CreateMeshData(std::string fileName, ID3D11Device * device, ID3D11DeviceContext * devContext);
	DirectX::XMFLOAT3 subtract(DirectX::XMFLOAT3 A, DirectX::XMFLOAT3 B);
	DirectX::XMFLOAT2 subtract(DirectX::XMFLOAT2 A, DirectX::XMFLOAT2 B);
};

