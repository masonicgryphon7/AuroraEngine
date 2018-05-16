#pragma once

#include"Vec3.h"

enum NODETYPE
{
	NONE_PATHABLE, PATHABLE, PATHABLE_CHECK
};

class Node
{
public:
	Node();

	~Node();
	//
	int g;
	int h;
	int f;
	NODETYPE pathable;
	Vec3 position;
	int parentX;
	int parentZ;
	


private:
	
};
