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
	MAX = gridWidth;
	grid = std::vector<std::vector<Node>>(gridWidth, std::vector<Node>(gridHeight));

	

}

cPathCreator::~cPathCreator()
{
}

void cPathCreator::trumpTheBorders()
{
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
}

void cPathCreator::addTerrain(std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> positions, int StartX, int StartY)
{
	for (int x = MIN; x < positions.size(); x++) //Row
	{
		for (int y = MIN; y <positions.size(); y++) //Column
		{

			grid[StartY + y][StartX + x].position = Vec3(positions[x][y].position.x+ StartY, positions[x][y].position.y, positions[x][y].position.z+ StartX);
			grid[StartY + y][StartX + x].g = 0;
			grid[StartY + y][StartX + x].f = 0;
			grid[StartY + y][StartX + x].h = 0;
			grid[StartY + y][StartX + x].parentX = -999999999;
			grid[StartY + y][StartX + x].parentZ = -999999999;
			grid[StartY + y][StartX + x].pathable = true;
		}
	}
	int i = 0;

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
