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

	void addGameObjectToTree(GameObject* gameObject);

private:
	std::vector<QuadTreeNode*> children;
	std::vector<GameObject*> gameObjects;
	OOBB OOBoundingBox;

};

