#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <DirectXMath.h>
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <d3d11.h>
#include <iostream>
#include <fstream>

class TerrainGenerator
{
private:

	struct HeightMapAttributes
	{
		int Width_Columns;
		int Height_Row;
		std::vector<std::vector<DirectX::XMFLOAT3>> VertInfo;
	};

	int grid_Row, grid_Column;
	bool HeightMapLoaded = false, HeightmapWithinBoundsOfGrid = false;
	std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> RealVertArr;

	std::vector<VERTEX_POS3UV2T3B3N3*> TriangleArr;
public:
	HeightMapAttributes HeightMapVariables;
	int vertCount;
	int vertpatchCount;

	TerrainGenerator();
	TerrainGenerator(int grid_RowPr, int grid_ColumnPr);
	TerrainGenerator(int grid_RowPr, int grid_ColumnPr, char* HeightMapFileName);
	~TerrainGenerator();

	void LoadHeightMapToAttributes(char* HeightMapFileName);

	int getVertCount() {return vertCount;};
	int getGrid_Row() { return grid_Row; };
	int getGrid_Column() { return grid_Column; };

	void loadRandomTerrainHeights();
	DirectX::XMFLOAT3 subtract(DirectX::XMFLOAT3 A, DirectX::XMFLOAT3 B);
	DirectX::XMFLOAT2 subtract(DirectX::XMFLOAT2 A, DirectX::XMFLOAT2 B);

	std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> getRealVertArr();

	std::vector<VERTEX_POS3UV2T3B3N3> getPatchTriangleArr(int StartX, int StartY, int endX, int endY);
	std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> PatchRealVertArr(int StartX, int StartY, int endX, int endY);
	int getPatchVertCount();

};
#endif