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

TerrainGenerator::TerrainGenerator(int grid_RowPr, int grid_ColumnPr, char* HeightMapFileName)
{
	grid_Row = grid_RowPr;
	grid_Column = grid_ColumnPr;
	LoadHeightMapToAttributes(HeightMapFileName);
	loadRandomTerrainHeights();
}

TerrainGenerator::~TerrainGenerator()
{
}

void TerrainGenerator::LoadHeightMapToAttributes(char* HeightMapFileName)
{
	FILE *tmpFile;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;
	unsigned char* bitMapImage_Info;
	int mapSize, bitMapIndx = 0, index;
	DirectX::XMFLOAT3 tempXMFLOAT3 = DirectX::XMFLOAT3(0, 0, 0);
	HeightMapLoaded = true;

	tmpFile = fopen(HeightMapFileName, "rb");
	if (!tmpFile)
	{
		std::cout << "Error: Couldn't find HeightMap." << std::endl;
		HeightMapLoaded = false;
	}

	//Initiate BitMap Headers (File, Info).
	fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, tmpFile);
	fread(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, tmpFile);
	HeightMapVariables.Height_Row = InfoHeader.biHeight;
	HeightMapVariables.Width_Columns = InfoHeader.biWidth;

	if (HeightMapVariables.Height_Row < grid_Row && HeightMapVariables.Width_Columns < grid_Column)
	{
		HeightmapWithinBoundsOfGrid = true;
	}
																				//RGB size (r-byte, g-byte, b-byte).
	mapSize = HeightMapVariables.Height_Row * HeightMapVariables.Width_Columns * 3;

	//Store Image Data.
	bitMapImage_Info = new unsigned char[mapSize]; //Set fitting size for array that will contain data.
	fseek(tmpFile, FileHeader.bfOffBits, SEEK_SET); //Pointer is set to the beginning of data.
	fread(bitMapImage_Info, 1, mapSize, tmpFile); //Store data in bitMapImage_Info.
	fclose(tmpFile);
	int a = HeightMapVariables.Height_Row;
	int b = HeightMapVariables.Width_Columns;
	for (int i = 0; i < HeightMapVariables.Height_Row; i++)
	{
		std::vector<DirectX::XMFLOAT3> tempHeightVert;

		for (int j = 0; j < HeightMapVariables.Width_Columns; j++)
		{

			tempXMFLOAT3 = DirectX::XMFLOAT3((float)i, ((float)bitMapImage_Info[bitMapIndx] / 10.0f /*Smoothing Value.*/), (float)j);
			bitMapIndx += 3;
			tempHeightVert.push_back(tempXMFLOAT3);
		}

		HeightMapVariables.VertInfo.push_back(tempHeightVert);

	}

	delete[] bitMapImage_Info;
	bitMapImage_Info = 0;

}

void TerrainGenerator::loadRandomTerrainHeights()
{
	srand(time(NULL));
	float g_posX = 0, g_posZ = 0;
	float srandNumber = 0.0f;
	VERTEX_POS3UV2T3B3N3 Vertices;
	DirectX::XMFLOAT3 vectSample[5];

	for (int j = 0; j < (grid_Row); j++)
	{
		std::vector<VERTEX_POS3UV2T3B3N3> TempVertArr;

		for (int i = 0; i < (grid_Column); i++)
		{
			srandNumber = (((float)rand() / float(RAND_MAX)) * 0.5f);

			Vertices.position = DirectX::XMFLOAT3(i, srandNumber, j);
			Vertices.uv = DirectX::XMFLOAT2(((float)i / (grid_Row-1)), ((float)j / (grid_Column-1)));
			Vertices.normal = DirectX::XMFLOAT3(0, 1, 0);
			TempVertArr.push_back(Vertices);
		}

		RealVertArr.push_back(TempVertArr);
	}

							// - - - HEIGHT MAP - - -		OBS OM BMP-HEIGHTMAPPEN ÄR MINDRE ÄN GRID SÅ KRASCHAR PROGRAMMET.
	if (HeightMapLoaded && !HeightmapWithinBoundsOfGrid)								  
	{
		for (int i = 0; i < grid_Row; i++)
		{
			for (int j = 0; j < grid_Column; j++)
			{
				RealVertArr[j][i].position.y = HeightMapVariables.VertInfo[i][j].y;
			}
		}
	}
							// - - - - - - - - - - -

	for (int i = 1; i < grid_Row -1; i++)
	{
		for (int j = 1; j < grid_Column -1; j++)
		{
			//Normal
			vectSample[0] = subtract(RealVertArr[i][j].position, RealVertArr[i][j + 1].position);
			vectSample[1] = subtract(RealVertArr[i][j].position, RealVertArr[i + 1][j].position);
			vectSample[2] = subtract(RealVertArr[i][j].position, RealVertArr[i][j -1].position);
			vectSample[3] = subtract(RealVertArr[i][j].position, RealVertArr[i - 1][j].position);
			vectSample[4] = vectSample[0];

			DirectX::XMVECTOR normalTemp = DirectX::XMVectorSet(0, 0, 0, 0);
			
			for (int k = 0; k < 4; k++)
			{
				normalTemp=DirectX::XMVectorAdd(normalTemp, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&vectSample[k + 1]), DirectX::XMLoadFloat3(&vectSample[k])));

			}
			normalTemp = DirectX::XMVector3Normalize(normalTemp);

			DirectX::XMStoreFloat3(&RealVertArr[i][j].normal, normalTemp);
		}
	}

	for (int indexX = 0; indexX < (grid_Row - 1); indexX++)
	{
		for (int indexZ = 0; indexZ < (grid_Column - 1); indexZ++)
		{
			TriangleArr.push_back(&RealVertArr[indexX + 1][indexZ]);
			TriangleArr.push_back(&RealVertArr[indexX + 1][indexZ + 1]);
			TriangleArr.push_back(&RealVertArr[indexX][indexZ]);

			TriangleArr.push_back(&RealVertArr[indexX + 1][indexZ + 1]);
			TriangleArr.push_back(&RealVertArr[indexX][indexZ + 1]);
			TriangleArr.push_back(&RealVertArr[indexX][indexZ]);
		}
	}

	for (int i = 0; i < TriangleArr.size(); i += 3)
	{
		DirectX::XMFLOAT3 vec1 = subtract(TriangleArr[i + 1]->position, TriangleArr[i]->position);
		DirectX::XMFLOAT3 vec2 = subtract(TriangleArr[i + 2]->position, TriangleArr[i]->position);

		DirectX::XMFLOAT2 uVec1 = subtract(TriangleArr[i + 1]->uv, TriangleArr[i]->uv);
		DirectX::XMFLOAT2 uVec2 = subtract(TriangleArr[i + 2]->uv, TriangleArr[i]->uv);

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
		DirectX::XMStoreFloat3(&TriangleArr[i]->tangent, tangent);
		DirectX::XMStoreFloat3(&TriangleArr[i + 1]->tangent, tangent);
		DirectX::XMStoreFloat3(&TriangleArr[i + 2]->tangent, tangent);

		DirectX::XMVECTOR bitangent = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&TriangleArr[i]->tangent), DirectX::XMLoadFloat3(&TriangleArr[i]->normal)));
		DirectX::XMStoreFloat3(&TriangleArr[i]->bitangent, bitangent);
		DirectX::XMStoreFloat3(&TriangleArr[i + 1]->bitangent, bitangent);
		DirectX::XMStoreFloat3(&TriangleArr[i + 2]->bitangent, bitangent);


	}

	vertCount = TriangleArr.size();

}

DirectX::XMFLOAT3 TerrainGenerator::subtract(DirectX::XMFLOAT3 A, DirectX::XMFLOAT3 B)
{
	DirectX::XMFLOAT3 tempVec;

	tempVec.x = A.x - B.x;
	tempVec.y = A.y - B.y;
	tempVec.z = A.z - B.z;

	return tempVec;
}

DirectX::XMFLOAT2 TerrainGenerator::subtract(DirectX::XMFLOAT2 A, DirectX::XMFLOAT2 B)
{
	DirectX::XMFLOAT2 tempVec;

	tempVec.x = A.x - B.x;
	tempVec.y = A.y - B.y;



	return tempVec;
}

std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> TerrainGenerator::getRealVertArr()
{
	return RealVertArr;
}

std::vector<VERTEX_POS3UV2T3B3N3> TerrainGenerator::getPatchTriangleArr(int StartX, int StartY, int endX, int endY)
{
	bool limiterX = 0, limiterY = 0;
	std::vector<VERTEX_POS3UV2T3B3N3> returnTriangleArr;

	if (endX == 300)
		limiterX = 1;
	if (endY == 300)
		limiterY = 1;

	for (int indexX = StartX; indexX < (endX - limiterX); indexX++)
	{
		for (int indexZ = StartY; indexZ < (endY - limiterY); indexZ++)
		{
			returnTriangleArr.push_back(RealVertArr[indexX + 1][indexZ]);

			returnTriangleArr.push_back(RealVertArr[indexX + 1][indexZ + 1]);
			returnTriangleArr.push_back(RealVertArr[indexX][indexZ]);

			returnTriangleArr.push_back(RealVertArr[indexX + 1][indexZ + 1]);
			returnTriangleArr.push_back(RealVertArr[indexX][indexZ + 1]);
			returnTriangleArr.push_back(RealVertArr[indexX][indexZ]);
		}
	}


	vertpatchCount = returnTriangleArr.size();

	return returnTriangleArr;
}

std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> TerrainGenerator::PatchRealVertArr(int startX, int startY, int endX, int endY)
{
	std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> returnValue;
	int i = 0, j = 0;

	for (int indexX = startX; indexX < (grid_Row); indexX++)
	{
		
		for (int indexZ = startY; indexZ < (grid_Column); indexZ++)
		{
			returnValue[i][j] = RealVertArr[indexX][indexZ];
			j++;
		}

		i++;
	}

	return returnValue;
}

int TerrainGenerator::getPatchVertCount()
{
	return vertpatchCount;
}
