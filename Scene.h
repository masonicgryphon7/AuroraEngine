#pragma once
#include "GameObject.h"
#include <vector>
#include "QuadTreeNode.h"
#include <DirectXMath.h>
#include "Camera.h"

class Scene
{
public:
	Scene();
	~Scene();

	GameObject* createEmptyGameObject();
	GameObject* createEmptyGameObject(DirectX::XMVECTOR position);
	GameObject* getGameObjectAt(int index);
	std::vector<GameObject*> getObjectsToRender(GameObject* camera);
	std::vector<GameObject*> getSceneObjects();
	void update();



private:
	std::vector<GameObject*>sceneObjects;
	std::vector<QuadTreeNode*> root;

	int planeAABBIntersect(OOBB otherOBB, DirectX::XMVECTOR otherPosition, DirectX::XMVECTOR frustumPlane);
	std::vector<GameObject*> frustumCull(GameObject * camera);

};

