#pragma once
#include "GameObject.h"
#include <vector>
#include "QuadTreeNode.h"
#include <DirectXMath.h>
#include "Camera.h"

struct PLANE
{
	DirectX::XMFLOAT3 normal;
	float distance;
};

static class Scene
{
public:
	Scene();
	~Scene();

	GameObject* createEmptyGameObject();
	GameObject* createEmptyGameObject(DirectX::XMVECTOR position);
	GameObject* getGameObjectAt(int index);
	std::vector<GameObject*> getObjectsToRender(GameObject* camera);
	std::vector<GameObject*> getSceneObjects();
	int getSceneObjectsCount();

	void update();



private:
	static std::vector<GameObject*>sceneObjects;
	std::vector<QuadTreeNode*> root;
	int frustumCheck(OOBB otherOBB, DirectX::XMVECTOR otherPosition, PLANE *planes);
	int planeAABBIntersect(OOBB otherOBB, DirectX::XMVECTOR otherPosition, DirectX::XMVECTOR frustumPlane);
	std::vector<GameObject*> frustumCull(GameObject * camera);

}gScene;

