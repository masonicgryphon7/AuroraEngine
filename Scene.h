#pragma once
#include "GameObject.h"
#include <vector>
#include "QuadTreeNode.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "json.hpp"
#include "Vector3.h"
#include <tuple>

struct JsonSceneWriter
{
	std::string name, filePath;
	Vector3 position;
	Vector3 rotation;

	JsonSceneWriter(const std::string& s, const std::string& path, Vector3 p, Vector3 r)
	{
		name = s;
		filePath = path;
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

	GameObject* CreateGameObject(Primitives primitive, Vector3 position = Vector3(0, 0, 0), Vector3 rotation = Vector3(0, 0, 0));
	GameObject* CreateGameObject(Mesh* mesh, Vector3 position = Vector3(0, 0, 0), Vector3 rotation = Vector3(0, 0, 0));

	GameObject* getGameObjectAt(int index);
	std::vector<GameObject*>* getFrustumCulledResult();
	std::vector<GameObject*> getSceneObjects();
	int getSceneObjectsCount();
	void destroyGameObjects();
	void frustumCull(GameObject * camera);

	void update();

	// SAVE_LOAD
	void SaveScene();
	void LoadScene();

	bool ContainsGUID(std::string guid);

	// FOR CLEAN SAKE
	std::string SetMeshPath(std::string str, const std::string& line);
	std::string SetObjectName(std::string str, const std::string& line);
	Vector3 SetPosition(Vector3 vec, const std::string& line);
	Vector3 SetRotation(Vector3 vec, const std::string& line);

	std::string Truncate(std::string line);
	std::string TruncateVector(std::string line, unsigned int type);

	std::string Encrypt(std::string msg, std::string const& key);
	std::string Decrypt(std::string const& msg, std::string const& key);

	std::string Compress(std::string str);
	std::string IntToString(int i);

	static GameObject* selectedGameObject;

private:
	std::vector<std::string> containedGUID; // contains all GUID's of current scene
	static std::vector<GameObject*> frustumCulledResult;
	static std::vector<GameObject*> sceneObjects;
	std::vector<QuadTreeNode*> root;
	int frustumCheck(OOBB otherOBB, DirectX::XMVECTOR otherPosition, PLANE *planes);
	int planeAABBIntersect(OOBB otherOBB, DirectX::XMVECTOR otherPosition, DirectX::XMVECTOR frustumPlane);

}gScene;

