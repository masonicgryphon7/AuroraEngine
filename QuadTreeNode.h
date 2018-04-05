#pragma once
#include <vector>
#include "GameObject.h"
#include "PRIMITIVE_GEOMETRY.h"

#define DEPTH_LEVEL 3

class QuadTreeNode
{
public:
	QuadTreeNode();
	~QuadTreeNode();

	void updateTree(std::vector<GameObject*> gameObjects);

private:
	std::vector<QuadTreeNode*> children;
	std::vector<GameObject*> gameObjects;
	OOBB OOBoundingBox;

	void createChildren(int depth, std::vector<GameObject*> gameObjects);
	void createOOBB(std::vector<GameObject*> gameObjects);
};

