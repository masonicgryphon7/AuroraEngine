#include "PathCreator.h"
#include "Debug.h"

std::vector<std::vector<Node>> cPathCreator::grid;

cPathCreator::cPathCreator()
{

}

cPathCreator::cPathCreator(std::vector<std::vector<std::vector<VERTEX_POS3UV2T3B3N3>>> positionsArr, int Width, int Height)
{
	/* begins		begins		begins		 begins
		0,0:tr1		100,0:tr2	0,100:tr3	 100,100:tr4

	*/

	//for (int s = 0; s < positionsArr.size(); s++)
	//{
	//	grid = std::vector<std::vector<Node>>(MAX, std::vector<Node>(MAX));
	//	for (int x = MIN; x < MAX; x++)
	//	{
	//		for (int y = MIN; y < MAX; y++)
	//		{
	//			grid[y][x].position = Vec3(positionsArr.at(s)[x][y].position.x, positionsArr.at(s)[x][y].position.y, positionsArr.at(s)[x][y].position.z);
	//			grid[y][x].g = 0;
	//			grid[y][x].f = 0;
	//			grid[y][x].h = 0;
	//			grid[y][x].parentX = -999999999;
	//			grid[y][x].parentZ = -999999999;
	//			grid[y][x].pathable = true;
	//		}
	//	}



		////border grid
		//for (int i = MIN; i < MAX; i++)
		//{
		//	grid[i][MIN].pathable = false;
		//	//display[i][MIN] = 1;

		//}
		//for (int i = MIN; i < MAX; i++)
		//{
		//	grid[i][MAX - 1].pathable = false;
		//	//display[i][MAX - 1] = 1;

		//}
		//for (int i = MIN; i < MAX; i++)
		//{
		//	grid[MIN][i].pathable = false;
		//	//display[MIN][i] = 1;

		//}
		//for (int i = MIN; i < MAX; i++)
		//{
		//	grid[MAX - 1][i].pathable = false;
		//	//display[MAX - 1][i] = 1;

		//}
	//}
}


cPathCreator::~cPathCreator()
{

}

void cPathCreator::createNodes(std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> positions, int minX, int minZ, int maxX, int maxZ)
{
	grid = std::vector<std::vector<Node>>(MAX, std::vector<Node>(MAX));
	int s = 0, z = 0;

	for (int x = minZ; x < maxZ; x++)
	{
		for (int y = minX; y < maxX; y++)
		{
			grid[y][x].position = Vec3(positions[s][z].position.x, positions[s][z].position.y, positions[s][z].position.z);
			grid[y][x].g = 0;
			grid[y][x].f = 0;
			grid[y][x].h = 0;
			grid[y][x].parentX = -999999999;
			grid[y][x].parentZ = -999999999;
			grid[y][x].pathable = true;

			s++;
		}
		z++;
	}

	//border grid
	for (int i = minX; i < maxX; i++)
	{
		grid[i][minX].pathable = false;
		//display[i][MIN] = 1;

	}
	for (int i = minX; i < maxX; i++)
	{
		grid[i][maxX - 1].pathable = false;
		//display[i][MAX - 1] = 1;

	}
	for (int i = minZ; i < maxZ; i++)
	{
		grid[minZ][i].pathable = false;
		//display[MIN][i] = 1;

	}
	for (int i = minZ; i < maxZ; i++)
	{
		grid[maxZ - 1][i].pathable = false;
		//display[MAX - 1][i] = 1;

	}
}

void cPathCreator::createNodes(std::vector<std::vector<std::vector<VERTEX_POS3UV2T3B3N3>>> positionsArr, int MAXg, int MINg, int TerrrainPatchSize)
{
	grid = std::vector<std::vector<Node>>(MAXg, std::vector<Node>(MAXg));	
	/*int patchXstart = 0, patchZstart = 0, patchXend = 0, patchZend = 0;*/
	int s = 0, z = 0;

	for (int s = 0; s < positionsArr.size(); s++)
	{
		//TerrainOne Z:0 X:0
		for (int x = MIN; x < MAX; x++)
		{
			for (int y = MIN; y < MAX; y++)
			{

				grid[y][x].position = Vec3(positionsArr.at(s)[x][y].position.x, positionsArr.at(s)[x][y].position.y, positionsArr.at(s)[x][y].position.z);
				grid[y][x].g = 0;
				grid[y][x].f = 0;
				grid[y][x].h = 0;
				grid[y][x].parentX = -999999999;
				grid[y][x].parentZ = -999999999;
				grid[y][x].pathable = true;

			}
		}
		//TerrainTwo X:99 Z:0
		for (int x = MIN; x < MAX; x++)
		{
			for (int y = MIN; y < MAX; y++)
			{

				grid[100 + y][x].position = Vec3(positionsArr.at(s)[x][y].position.x, positionsArr.at(s)[x][y].position.y, positionsArr.at(s)[x][y].position.z);
				grid[100 + y][x].g = 0;
				grid[100 + y][x].f = 0;
				grid[100 + y][x].h = 0;
				grid[100 + y][x].parentX = -999999999;
				grid[100 + y][x].parentZ = -999999999;
				grid[100 + y][x].pathable = true;

			}
		}
		//TerrainThree Z:99 X:99
		for (int x = MIN; x < MAX; x++)
		{
			for (int y = MIN; y < MAX; y++)
			{

				grid[100 + y][100 + x].position = Vec3(positionsArr.at(s)[x][y].position.x, positionsArr.at(s)[x][y].position.y, positionsArr.at(s)[x][y].position.z);
				grid[100 + y][100 + x].g = 0;
				grid[100 + y][100 + x].f = 0;
				grid[100 + y][100 + x].h = 0;
				grid[100 + y][100 + x].parentX = -999999999;
				grid[100 + y][100 + x].parentZ = -999999999;
				grid[100 + y][100 + x].pathable = true;

			}
		}
		//TerrainFour X:00 Z:99
		for (int x = MIN; x < MAX; x++)
		{
			for (int y = MIN; y < MAX; y++)
			{

				grid[y][100 + x].position = Vec3(positionsArr.at(s)[x][y].position.x, positionsArr.at(s)[x][y].position.y, positionsArr.at(s)[x][y].position.z);
				grid[y][100 + x].g = 0;
				grid[y][100 + x].f = 0;
				grid[y][100 + x].h = 0;
				grid[y][100 + x].parentX = -999999999;
				grid[y][100 + x].parentZ = -999999999;
				grid[y][100 + x].pathable = true;

			}
		}
		//JAG FÅR HYBRIS PÅ HÅRDKODNING.

		//border grid
		for (int i = MINg; i < MAXg; i++)
		{
			grid[i][MIN].pathable = false;
			//display[i][MIN] = 1;

		}
		for (int i = MINg; i < MAXg; i++)
		{
			grid[i][MAXg - 1].pathable = false;
			//display[i][MAX - 1] = 1;

		}
		for (int i = MINg; i < MAXg; i++)
		{
			grid[MIN][i].pathable = false;
			//display[MIN][i] = 1;

		}
		for (int i = MINg; i < MAXg; i++)
		{
			grid[MAX - 1][i].pathable = false;
			//display[MAX - 1][i] = 1;

		}
	}
}

void cPathCreator::createNodes(std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> positions)
{
	//std::vector<Node> result;
	//Node grid[GRID][GRID];
	//int display[GRID][GRID];
	//pos
	grid = std::vector<std::vector<Node>>(MAX, std::vector<Node>(MAX));
	for (int x = MIN; x < MAX; x++)
	{
		for (int y = MIN; y < MAX; y++)
		{
				 
			grid[y][x].position = Vec3(positions[x][y].position.x, positions[x][y].position.y, positions[x][y].position.z);
			grid[y][x].g = 0;
			grid[y][x].f = 0;
			grid[y][x].h = 0;
			grid[y][x].parentX = -999999999;
			grid[y][x].parentZ = -999999999;
			grid[y][x].pathable = true;

			//display[x][y] = 0;
		}
	}

	//border grid
	for (int i = MIN; i < MAX; i++)
	{
		grid[i][MIN].pathable = false;
		//display[i][MIN] = 1;

	}
	for (int i = MIN; i < MAX; i++)
	{
		grid[i][MAX - 1].pathable = false;
		//display[i][MAX - 1] = 1;

	}
	for (int i = MIN; i < MAX; i++)
	{
		grid[MIN][i].pathable = false;
		//display[MIN][i] = 1;

	}
	for (int i = MIN; i < MAX; i++)
	{
		grid[MAX - 1][i].pathable = false;
		//display[MAX - 1][i] = 1;

	}


	////fail test
	//for (int i = MIN; i < MAX - 2; i++)
	//{
	//	grid[i][3].pathable = false;
	//	//display[i][3] = 1;

	//}
}

std::vector<Node> cPathCreator::getPath(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 goalPos)
{
	std::vector<std::vector<Node>> tempGrid = grid;

	Node goalNode = tempGrid[std::round(startPos.x)][std::round(startPos.z)];  // får aldrig vara -1. samplar utanför terrain array
	Node startNode = tempGrid[std::round(goalPos.x)][std::round(goalPos.z)];

	if (startNode.pathable == false) {
		 startNode.position= goalNode.position;
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


	//
	std::vector<Node> resultNodes = std::vector<Node>();
	std::vector<Node> openNodes = std::vector<Node>();
	std::vector<Node> closedNodes = std::vector<Node>();
	openNodes.push_back(startNode);
	bool succes = false;
	Node currentNode = Node();
	int i = 0;
	while (openNodes.size() > 0 && succes == false) {

		float lowestF = -1;
		int lowestFIndex = openNodes[0].f;

		for (int i = 0; i < openNodes.size(); i++)
		{

			if (openNodes[i].f < lowestF || lowestF == -1) {
				lowestF = openNodes[i].f;
				lowestFIndex = i;

			}
		}
		closedNodes.push_back(openNodes.at(lowestFIndex));
		currentNode = closedNodes.back();
		openNodes.erase(openNodes.begin() + lowestFIndex);

		if (closedNodes.back().position == goalNode.position) {
			succes = true;
			break;
		}

		//for each neighbor
		for (int i = 0; i < 9; i++)
		{
			//neighbor index
			int x = currentNode.position.x + xArr[i];
			int z = currentNode.position.z + zArr[i];
			//std::cout << "x" << x << " z" << z << std::endl;

			bool isViableNeighbor = true;
			if (tempGrid[x][z].pathable == false) {
				isViableNeighbor = false;

			}

			for (int j = 0; j < closedNodes.size(); j++)
			{
				if (tempGrid[x][z].position == closedNodes[j].position) {
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
		i++;
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

		resultNodes.push_back(goalNode);
	}

	if (reversePath) {
		std::reverse(resultNodes.begin(), resultNodes.end());
	}

	return resultNodes;
}

float cPathCreator::manhattan(Node first, Node second)
{
	float result = 10 * (abs(first.position.x - second.position.x) + abs(first.position.z - second.position.z));

	return result;
}
