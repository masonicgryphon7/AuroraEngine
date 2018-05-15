#include "PathCreator.h"
#include "Debug.h"

std::vector<std::vector<Node>> cPathCreator::grid;
int cPathCreator::MAX;
int cPathCreator::MIN;

cPathCreator::cPathCreator()
{

}

cPathCreator::cPathCreator(int gridWidth, int gridHeight)
{
	MAX = gridWidth; //+ (gridWidth - 1);
	grid = std::vector<std::vector<Node>>(MAX, std::vector<Node>(MAX));

}

cPathCreator::~cPathCreator()
{
}

void cPathCreator::trumpTheBorders()
{
	//border grid
	for (int i = MIN; i < MAX; i++)
	{
		grid[i][MIN].pathable = NONE_PATHABLE;
		//display[i][MIN] = 1;

	}
	for (int i = MIN; i < MAX; i++)
	{
		grid[i][MAX - 1].pathable = NONE_PATHABLE;
		//display[i][MAX - 1] = 1;

	}
	for (int i = MIN; i < MAX; i++)
	{
		grid[MIN][i].pathable = NONE_PATHABLE;
		//display[MIN][i] = 1;

	}
	for (int i = MIN; i < MAX; i++)
	{
		grid[MAX - 1][i].pathable = NONE_PATHABLE;
		//display[MAX - 1][i] = 1;

	}
}

void cPathCreator::addTerrain(std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> positions, int StartX, int StartY)
{
	for (int x = MIN; x < positions.size(); x++) //Row
	{
		for (int y = MIN; y < positions.size(); y++) //Column
		{

			grid[StartY + y][StartX + x].position = Vec3(positions[x][y].position.x + StartY, positions[x][y].position.y, positions[x][y].position.z + StartX);
			grid[StartY + y][StartX + x].g = 0;
			grid[StartY + y][StartX + x].f = 0;
			grid[StartY + y][StartX + x].h = 0;
			grid[StartY + y][StartX + x].parentX = -999999999;
			grid[StartY + y][StartX + x].parentZ = -999999999;
			grid[StartY + y][StartX + x].pathable = PATHABLE;

			////top left
			//grid[StartY + y][StartX + x].position = Vec3(positions[x][y].position.x+ StartY, positions[x][y].position.y, positions[x][y].position.z+ StartX);
			//grid[StartY + y][StartX + x].g = 0;
			//grid[StartY + y][StartX + x].f = 0;
			//grid[StartY + y][StartX + x].h = 0;
			//grid[StartY + y][StartX + x].parentX = -999999999;
			//grid[StartY + y][StartX + x].parentZ = -999999999;
			//grid[StartY + y][StartX + x].pathable = PATHABLE;

			////top middle, lerp
			//DirectX::XMVECTOR lerpValue1 = DirectX::XMVectorLerp(DirectX::XMVectorSet(positions[x][y].position.x + StartY, positions[x][y].position.y, positions[x][y].position.z + StartX, 0.0), DirectX::XMVectorSet(positions[x][y + 1].position.x + StartY, positions[x][y + 1].position.y, positions[x][y + 1].position.z + StartX, 0.0), 0.5);
			//grid[StartY + y][StartX + x + 1].position = Vec3(DirectX::XMVectorGetX(lerpValue1), DirectX::XMVectorGetY(lerpValue1), DirectX::XMVectorGetZ(lerpValue1));
			//grid[StartY + y][StartX + x + 1].g = 0;
			//grid[StartY + y][StartX + x + 1].f = 0;
			//grid[StartY + y][StartX + x + 1].h = 0;
			//grid[StartY + y][StartX + x + 1].parentX = -999999999;
			//grid[StartY + y][StartX + x + 1].parentZ = -999999999;
			//grid[StartY + y][StartX + x + 1].pathable = PATHABLE;
			//
			////top right
			//grid[StartY + y][StartX + x + 2].position = Vec3(positions[x][y + 1].position.x+ StartY, positions[x][y + 1].position.y, positions[x][y + 1].position.z+ StartX);
			//grid[StartY + y][StartX + x + 2].g = 0;
			//grid[StartY + y][StartX + x + 2].f = 0;
			//grid[StartY + y][StartX + x + 2].h = 0;
			//grid[StartY + y][StartX + x + 2].parentX = -999999999;
			//grid[StartY + y][StartX + x + 2].parentZ = -999999999;
			//grid[StartY + y][StartX + x + 2].pathable = PATHABLE;

			////middle left, lerp
			//DirectX::XMVECTOR lerpValue2 = DirectX::XMVectorLerp(DirectX::XMVectorSet(positions[x][y].position.x + StartY, positions[x][y].position.y, positions[x][y].position.z + StartX, 0.0), DirectX::XMVectorSet(positions[x + 1][y].position.x + StartY, positions[x + 1][y].position.y, positions[x + 1][y].position.z + StartX, 0.0), 0.5);
			//grid[StartY + y + 1][StartX + x].position = Vec3(DirectX::XMVectorGetX(lerpValue2), DirectX::XMVectorGetY(lerpValue2), DirectX::XMVectorGetZ(lerpValue2));
			//grid[StartY + y + 1][StartX + x].g = 0;
			//grid[StartY + y + 1][StartX + x].f = 0;
			//grid[StartY + y + 1][StartX + x].h = 0;
			//grid[StartY + y + 1][StartX + x].parentX = -999999999;
			//grid[StartY + y + 1][StartX + x].parentZ = -999999999;
			//grid[StartY + y + 1][StartX + x].pathable = PATHABLE;
			//
			////middle middle, lerp
			//DirectX::XMVECTOR lerpValue3 = DirectX::XMVectorLerp(DirectX::XMVectorSet(positions[x][y].position.x + StartY, positions[x][y].position.y, positions[x][y].position.z + StartX, 0.0), DirectX::XMVectorSet(positions[x + 1][y + 1].position.x + StartY, positions[x + 1][y + 1].position.y, positions[x + 1][y + 1].position.z + StartX, 0.0), 0.5);
			//grid[StartY + y + 1][StartX + x + 1].position = Vec3(DirectX::XMVectorGetX(lerpValue3), DirectX::XMVectorGetY(lerpValue3), DirectX::XMVectorGetZ(lerpValue3));
			//grid[StartY + y + 1][StartX + x + 1].g = 0;
			//grid[StartY + y + 1][StartX + x + 1].f = 0;
			//grid[StartY + y + 1][StartX + x + 1].h = 0;
			//grid[StartY + y + 1][StartX + x + 1].parentX = -999999999;
			//grid[StartY + y + 1][StartX + x + 1].parentZ = -999999999;
			//grid[StartY + y + 1][StartX + x + 1].pathable = PATHABLE;
			//
			////middle right, lerp
			//DirectX::XMVECTOR lerpValue4 = DirectX::XMVectorLerp(DirectX::XMVectorSet(positions[x][y + 1].position.x + StartY, positions[x][y + 1].position.y, positions[x][y + 1].position.z + StartX, 0.0), DirectX::XMVectorSet(positions[x + 1][y + 1].position.x + StartY, positions[x + 1][y + 1].position.y, positions[x + 1][y + 1].position.z + StartX, 0.0), 0.5);
			//grid[StartY + y + 1][StartX + x + 2].position = Vec3(DirectX::XMVectorGetX(lerpValue4), DirectX::XMVectorGetY(lerpValue4), DirectX::XMVectorGetZ(lerpValue4));
			//grid[StartY + y + 1][StartX + x + 2].g = 0;
			//grid[StartY + y + 1][StartX + x + 2].f = 0;
			//grid[StartY + y + 1][StartX + x + 2].h = 0;
			//grid[StartY + y + 1][StartX + x + 2].parentX = -999999999;
			//grid[StartY + y + 1][StartX + x + 2].parentZ = -999999999;
			//grid[StartY + y + 1][StartX + x + 2].pathable = PATHABLE;

			////bottom left
			//grid[StartY + y + 2][StartX + x].position = Vec3(positions[x + 1][y].position.x+ StartY, positions[x + 1][y].position.y, positions[x + 1][y].position.z+ StartX);
			//grid[StartY + y + 2][StartX + x].g = 0;
			//grid[StartY + y + 2][StartX + x].f = 0;
			//grid[StartY + y + 2][StartX + x].h = 0;
			//grid[StartY + y + 2][StartX + x].parentX = -999999999;
			//grid[StartY + y + 2][StartX + x].parentZ = -999999999;
			//grid[StartY + y + 2][StartX + x].pathable = PATHABLE;
			//
			////bottom middle, lerp
			//DirectX::XMVECTOR lerpValue5 = DirectX::XMVectorLerp(DirectX::XMVectorSet(positions[x + 1][y].position.x + StartY, positions[x + 1][y].position.y, positions[x + 1][y].position.z + StartX, 0.0), DirectX::XMVectorSet(positions[x + 1][y + 1].position.x + StartY, positions[x + 1][y + 1].position.y, positions[x + 1][y + 1].position.z + StartX, 0.0), 0.5);
			//grid[StartY + y + 2][StartX + x + 1].position = Vec3(DirectX::XMVectorGetX(lerpValue5), DirectX::XMVectorGetY(lerpValue5), DirectX::XMVectorGetZ(lerpValue5));
			//grid[StartY + y + 2][StartX + x + 1].g = 0;
			//grid[StartY + y + 2][StartX + x + 1].f = 0;
			//grid[StartY + y + 2][StartX + x + 1].h = 0;
			//grid[StartY + y + 2][StartX + x + 1].parentX = -999999999;
			//grid[StartY + y + 2][StartX + x + 1].parentZ = -999999999;
			//grid[StartY + y + 2][StartX + x + 1].pathable = PATHABLE;
			//
			////bottom right
			//grid[StartY + y + 2][StartX + x + 2].position = Vec3(positions[x + 1][y + 1].position.x+ StartY, positions[x + 1][y + 1].position.y, positions[x + 1][y + 1].position.z+ StartX);
			//grid[StartY + y + 2][StartX + x + 2].g = 0;
			//grid[StartY + y + 2][StartX + x + 2].f = 0;
			//grid[StartY + y + 2][StartX + x + 2].h = 0;
			//grid[StartY + y + 2][StartX + x + 2].parentX = -999999999;
			//grid[StartY + y + 2][StartX + x + 2].parentZ = -999999999;
			//grid[StartY + y + 2][StartX + x + 2].pathable = PATHABLE;

		}
	}
	int i = 0;
	blockGrid(DirectX::XMFLOAT3(20, 1, 1));
	loadBlockMap();
}
//
////void cPathCreator::createNodes(std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> positions)
//{
//	//std::vector<Node> result;
//	//Node grid[GRID][GRID];
//	//int display[GRID][GRID];
//	//pos
//	grid = std::vector<std::vector<Node>>(MAX, std::vector<Node>(MAX));
//	for (int x = MIN; x < MAX; x++)
//	{
//		for (int y = MIN; y < MAX; y++)
//		{
//				 
//			grid[y][x].position = Vec3(positions[x][y].position.x, positions[x][y].position.y, positions[x][y].position.z);
//			grid[y][x].g = 0;
//			grid[y][x].f = 0;
//			grid[y][x].h = 0;
//			grid[y][x].parentX = -999999999;
//			grid[y][x].parentZ = -999999999;
//			grid[y][x].pathable = true;
//
//			//display[x][y] = 0;
//		}
//	}
//
//	//border grid
//	for (int i = MIN; i < MAX; i++)
//	{
//		grid[i][MIN].pathable = false;
//		//display[i][MIN] = 1;
//
//	}
//	for (int i = MIN; i < MAX; i++)
//	{
//		grid[i][MAX - 1].pathable = false;
//		//display[i][MAX - 1] = 1;
//
//	}
//	for (int i = MIN; i < MAX; i++)
//	{
//		grid[MIN][i].pathable = false;
//		//display[MIN][i] = 1;
//
//	}
//	for (int i = MIN; i < MAX; i++)
//	{
//		grid[MAX - 1][i].pathable = false;
//		//display[MAX - 1][i] = 1;
//
//	}
//
//
//	////fail test
//	//for (int i = MIN; i < MAX - 2; i++)
//	//{
//	//	grid[i][3].pathable = false;
//	//	//display[i][3] = 1;
//
//	//}
//}

void cPathCreator::blockGrid(DirectX::XMFLOAT3 pos)
{
	//grid[(int)pos.x][(int)pos.z].pathable = PATHABLE_CHECK;
}

void cPathCreator::loadBlockMap()
{

	struct HeightMapAttributes
	{
		int Width_Columns;
		int Height_Row;
		std::vector<std::vector<DirectX::XMFLOAT3>> VertInfo;
	};
	HeightMapAttributes HeightMapVariables;
	FILE *tmpFile;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;
	unsigned char* bitMapImage_Info;
	int mapSize, bitMapIndx = 0, index;
	DirectX::XMFLOAT3 tempXMFLOAT3 = DirectX::XMFLOAT3(0, 0, 0);


	tmpFile = fopen("Assets/Block_Map2.bmp", "rb");
	if (!tmpFile)
	{
		std::cout << "Error: Couldn't find HeightMap." << std::endl;

	}

	//Initiate BitMap Headers (File, Info).
	fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, tmpFile);
	fread(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, tmpFile);
	HeightMapVariables.Height_Row = InfoHeader.biHeight;
	HeightMapVariables.Width_Columns = InfoHeader.biWidth;

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

	for (int i = 0; i < HeightMapVariables.Height_Row; i++)
	{


		for (int j = 0; j < HeightMapVariables.Width_Columns; j++)
		{
			int e = HeightMapVariables.VertInfo[i][j].y;
			if (e == 0)
				grid[i][j].pathable = NONE_PATHABLE;
		}


	}

	delete[] bitMapImage_Info;
	bitMapImage_Info = 0;
}

std::vector<Node> cPathCreator::getPath(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 goalPos)
{

	std::clock_t start;
	start = std::clock();

	std::vector<std::vector<Node>> tempGrid = grid;
	Node goalNode = tempGrid[std::round(startPos.x)][std::round(startPos.z)];  // f r aldrig vara -1. samplar utanf r terrain array
	Node startNode = tempGrid[std::round(goalPos.x)][std::round(goalPos.z)];
	//int maxIterationsAllowed = DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&startPos), DirectX::XMLoadFloat3(&goalPos))))*2;
	int iteration = 0;

	if (startNode.pathable == NONE_PATHABLE) {
		startNode.position = goalNode.position;
		std::vector<Node> resultNodes = std::vector<Node>();
		resultNodes.push_back(startNode);
		return resultNodes;
	}

	if (goalNode.pathable == NONE_PATHABLE) {
		startNode.position = startNode.position;
		std::vector<Node> resultNodes = std::vector<Node>();
		resultNodes.push_back(startNode);
		return resultNodes;
	}


	//direction help test
	bool reversePath = false;
	DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMVectorSet(goalNode.position.x, goalNode.position.y, goalNode.position.z, 0), DirectX::XMVectorSet(startNode.position.x, startNode.position.y, startNode.position.z, 0));
	if (DirectX::XMVectorGetW(DirectX::XMVector3Dot(direction, DirectX::XMVectorSet(1, 0, 1, 0)))>EPSILON) {
		Node temp = goalNode;
		goalNode = startNode;
		startNode = temp;
		reversePath = true;
	}



	std::vector<Node> resultNodes = std::vector<Node>();
	std::vector<Node> openNodes = std::vector<Node>();
	std::vector<std::vector<Node>> closedNodes = std::vector<std::vector<Node>>(MAX, std::vector<Node>(MAX));
	openNodes.push_back(startNode);
	bool succes = false;
	Node currentNode = Node();
	int iterations = 0;





	while (openNodes.size() > 0 && succes == false) {
		iteration++;
		float lowestF = -1;
		int lowestFIndex = openNodes[0].f;

		for (int i = 0; i < openNodes.size(); i++)
		{

			if (openNodes[i].f < lowestF || lowestF == -1) {
				lowestF = openNodes[i].f;
				lowestFIndex = i;

			}
		}

		Node tempNode = openNodes.at(lowestFIndex);
		closedNodes[tempNode.position.x][tempNode.position.z]=tempNode;
		currentNode = tempNode;
		openNodes.erase(openNodes.begin() + lowestFIndex);

		if (tempNode.position == goalNode.position) {
			succes = true;
			break;
		}

		//for each neighbor
		for (int i = 0; i < 9; i++)
		{
			if (i == 5) continue;

			iterations++;

			//neighbor index
			int x = currentNode.position.x + xArr[i];
			int z = currentNode.position.z + zArr[i];
			//std::cout << "x" << x << " z" << z << std::endl;

			bool isViableNeighbor = true;
			if (tempGrid[x][z].pathable == NONE_PATHABLE) {
				isViableNeighbor = false;

			}
			if (tempGrid[x][z].pathable == PATHABLE_CHECK && tempGrid[x][z].position != goalNode.position) {
				isViableNeighbor = false;
			}
			for (int j = 0; j < closedNodes.size(); j++)
			{
				if (tempGrid[x][z].position == closedNodes[x][z].position) {
					isViableNeighbor = false;

				}
			}



			if (isViableNeighbor == true) {
				bool isInOpenNodes = false;
				int index = -1;

				for (int j = 0; j < openNodes.size(); j++)
				{
					if (openNodes[j].position == tempGrid[x][z].position) {
						isInOpenNodes = true;
					}
				}

				if (isInOpenNodes == false) {
					tempGrid[x][z].h = manhattan(tempGrid[x][z], goalNode);
					if (i == 0 || i == 2 || i == 5 || i == 7) {
						tempGrid[x][z].g = 14 + currentNode.g;
					}
					else
					{
						tempGrid[x][z].g = 10 + currentNode.g;
					}
					tempGrid[x][z].f = tempGrid[x][z].g + tempGrid[x][z].h;
					tempGrid[x][z].parentX = currentNode.position.x;
					tempGrid[x][z].parentZ = currentNode.position.z;
					openNodes.push_back(tempGrid[x][z]);

				}
				else {
					int tempG = 0;
					if (i == 0 || i == 2 || i == 5 || i == 7) {
						tempG = 14 + currentNode.g;
					}
					else
					{
						tempG = 10 + currentNode.g;
					}
					if (tempGrid[x][z].g < tempG) {

						tempGrid[x][z].g = tempG;
						tempGrid[x][z].f = tempG + tempGrid[x][z].h;
						tempGrid[x][z].parentX = currentNode.position.x;
						tempGrid[x][z].parentZ = currentNode.position.z;
					}
				}


			}


		}
	}
	//Debug.Log("AStar Iterations:", i);

	int gg = 0;
	if (succes == true) {

		while (currentNode.parentX != -999999999)
		{


			resultNodes.push_back(currentNode);
			currentNode = tempGrid[currentNode.parentX][currentNode.parentZ];


			gg++;

		}


	}
	else {
		if (reversePath)
			resultNodes.push_back(startNode);
		else
		{
			resultNodes.push_back(goalNode);

		}
	}

	if (reversePath) {
		std::reverse(resultNodes.begin(), resultNodes.end());
	}
	Debug.Log("NINELOOP7: ", ((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000)));
	return resultNodes;
}

float cPathCreator::manhattan(Node first, Node second)
{
	float result = 10 * (abs(first.position.x - second.position.x) + abs(first.position.z - second.position.z));

	return result;
}