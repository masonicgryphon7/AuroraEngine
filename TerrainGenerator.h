#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <DirectXMath.h>
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <d3d11.h>

class TerrainGenerator
{

	//Include the possibility that other meshes cannot intersect with terrain.
	//Collision, so to speak. Search more info about how on the internet. (Usage of intersect function?)

private:

	int grid_Row, grid_Column/*, vertCount*/;


	//VertexType Variable; An variable that contains info about vertices.

public:
	std::vector<VERTEX_POS3UV2T3B3N3> TriangleArr;
	std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> RealVertArr;
	int vertCount;

	TerrainGenerator();
	TerrainGenerator(int grid_RowPr, int grid_ColumnPr);
	~TerrainGenerator();

	int getVertCount() {return vertCount;};
	void loadRandomTerrainHeights();

};
#endif