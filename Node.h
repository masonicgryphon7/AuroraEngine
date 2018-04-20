#pragma once

#include"Vec3.h"

class Node
{
public:
	Node();

	~Node();

	int g;
	int h;
	int f;
	bool pathable;
	Vec3 position;
	int parentX;
	int parentZ;
private:
	
};
