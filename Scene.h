#pragma once
#include "GameObject.h"
#include <vector>
#include "QuadTreeNode.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "json.hpp"
#include "Vector3.h"

struct JsonSceneWriter
{
	std::string name;
	Vector3 position;
	Vector3 rotation;

	JsonSceneWriter(const std::string s, Vector3 p, Vector3 r)
	{
		name = s;
		position = p;
		rotation = r;
	}
};

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
	std::vector<GameObject*> getFrustumCulledResult();
	std::vector<GameObject*> getSceneObjects();
	int getSceneObjectsCount();
	std::vector<GameObject*> frustumCull(GameObject * camera);

	void update();

	// SAVE_LOAD
	void SaveScene();
	void LoadScene();


	static GameObject* selectedGameObject;

private:
	std::vector<GameObject*> frustumCulledResult;
	static std::vector<GameObject*>sceneObjects;
	std::vector<QuadTreeNode*> root;
	int frustumCheck(OOBB otherOBB, DirectX::XMVECTOR otherPosition, PLANE *planes);
	int planeAABBIntersect(OOBB otherOBB, DirectX::XMVECTOR otherPosition, DirectX::XMVECTOR frustumPlane);

}gScene;

