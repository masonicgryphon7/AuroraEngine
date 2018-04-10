#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator()
{
	grid_Row = 10;
	grid_Column = 10;
	loadRandomTerrainHeights();
}

TerrainGenerator::TerrainGenerator(int grid_RowPr, int grid_ColumnPr)
{
	grid_Row = grid_RowPr;
	grid_Column = grid_ColumnPr;
	loadRandomTerrainHeights();
}

TerrainGenerator::~TerrainGenerator()
{
}

void TerrainGenerator::loadRandomTerrainHeights()
{
	srand(time(NULL));
	unsigned long* indices;
	float g_posX, g_posZ;
	int tempIndx = 0;
	float srandNumber = 0.0f;
	VERTEX_POS3UV2T3B3N3 Vertices;

	for (int j = 0; j < (grid_Row); j++)
	{
		std::vector<VERTEX_POS3UV2T3B3N3> TempVertArr;

		for (int i = 0; i < (grid_Column); i++)
		{
			Vertices.position = DirectX::XMFLOAT3(i, 0, j);
			Vertices.uv = DirectX::XMFLOAT2(((float)i / grid_Row), ((float)j / grid_Column));
			TempVertArr.push_back(Vertices);
		}
		RealVertArr.push_back(TempVertArr);
	}

	for (int i = 0; i < grid_Row; i++)
	{
		for (int j = 0; j < grid_Column; j++)
		{
			srandNumber = (((float)rand() / float(RAND_MAX)) * 0.2f);
			RealVertArr[i][j].position.y = srandNumber;
			RealVertArr[i][j].bitangent = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			RealVertArr[i][j].tangent = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			RealVertArr[i][j].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}


	for (int indexX = 0; indexX < (grid_Row - 1); indexX++)
	{
		for (int indexZ = 0; indexZ < (grid_Column - 1); indexZ++)
		{
			TriangleArr.push_back(RealVertArr[indexX + 1][indexZ]);
			TriangleArr.push_back(RealVertArr[indexX + 1][indexZ + 1]);
			TriangleArr.push_back(RealVertArr[indexX][indexZ]);

			TriangleArr.push_back(RealVertArr[indexX + 1][indexZ + 1]);
			TriangleArr.push_back(RealVertArr[indexX][indexZ + 1]);
			TriangleArr.push_back(RealVertArr[indexX][indexZ]);
		}
	}

	vertCount = TriangleArr.size();

}

