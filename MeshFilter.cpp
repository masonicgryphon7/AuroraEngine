#include "MeshFilter.h"



MeshFilter::MeshFilter()
{
}

MeshFilter::MeshFilter(Mesh * mesh):Component(-1, "Mesh Filter")
{
	this->mesh = mesh;
	int t = 0;
}


MeshFilter::~MeshFilter()
{
}

