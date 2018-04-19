#include "PathCreator.h"

std::vector<std::vector<Node>> cPathCreator::grid;

cPathCreator::cPathCreator()
{
}


cPathCreator::~cPathCreator()
{
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


	//fail test
	for (int i = MIN; i < MAX - 2; i++)
	{
		grid[i][3].pathable = false;
		//display[i][3] = 1;

	}
}

std::vector<Node> cPathCreator::getPath(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 goalPos)
{
	Node startNode = grid[std::round(startPos.x)][std::round(startPos.z)];  // får aldrig vara -1. samplar utanför terrain array
	Node goalNode = grid[std::round(goalPos.x)][std::round(goalPos.z)];

	std::vector<Node> resultNodes = std::vector<Node>();
	std::vector<Node> openNodes = std::vector<Node>();
	std::vector<Node> closedNodes = std::vector<Node>();
	openNodes.push_back(startNode);
	bool succes = false;
	Node *currentNode = new Node();

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
		currentNode = &closedNodes.back();
		openNodes.erase(openNodes.begin() + lowestFIndex);

		if (closedNodes.back().position == goalNode.position) {
			succes = true;

			break;
		}

		//for each neighbor
		for (int i = 0; i < 9; i++)
		{
			//neighbor index
			int x = currentNode->position.x + xArr[i];
			int z = currentNode->position.z + zArr[i];
			//std::cout << "x" << x << " y" << y << std::endl;

			bool isViableNeighbor = true;
			if (grid[x][z].pathable == false) {
				isViableNeighbor = false;

			}

			for (int j = 0; j < closedNodes.size(); j++)
			{
				if (grid[x][z].position == closedNodes[j].position) {
					isViableNeighbor = false;

				}
			}



			if (isViableNeighbor == true) {
				bool isInOpenNodes = false;
				int index = -1;

				for (int j = 0; j < openNodes.size(); j++)
				{
					if (openNodes[j].position == grid[x][z].position) {
						isInOpenNodes = true;
					}
				}

				if (isInOpenNodes == false) {
					grid[x][z].h = manhattan(grid[x][z], goalNode);
					if (i == 0 || i == 2 || i == 5 || i == 7) {
						grid[x][z].g = 14 + currentNode->g;
					}
					else
					{
						grid[x][z].g = 10 + currentNode->g;
					}
					grid[x][z].f = grid[x][z].g + grid[x][z].h;
					grid[x][z].parentX = currentNode->position.x;
					grid[x][z].parentZ = currentNode->position.z;
					openNodes.push_back(grid[x][z]);

				}
				else {
					int tempG = 0;
					if (i == 0 || i == 2 || i == 5 || i == 7) {
						tempG = 14 + currentNode->g;
					}
					else
					{
						tempG = 10 + currentNode->g;
					}
					if (grid[x][z].g < tempG) {
								
						grid[x][z].g = tempG;
						grid[x][z].f = tempG + grid[x][z].h;
						grid[x][z].parentX = currentNode->position.x;
						grid[x][z].parentZ = currentNode->position.z;
					}
				}


			}

		}
	}
	int gg = 0;
	if (succes == true) {

		while (currentNode->parentX != -999999999)
		{


			resultNodes.push_back(*currentNode);
			currentNode = &grid[currentNode->parentX][currentNode->parentZ];


			gg++;

		}


	}
	else {
		//failed
	}

	return resultNodes;
}

float cPathCreator::manhattan(Node first, Node second)
{
	float result = 10 * (abs(first.position.x - second.position.x) + abs(first.position.y - second.position.y));

	return result;
}
