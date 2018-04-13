#include "Scene.h"
std::vector<GameObject*> Scene::sceneObjects;
GameObject* Scene::selectedGameObject;

Scene::Scene()
{
}


Scene::~Scene()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{

		delete sceneObjects[i];
	}
}

GameObject * Scene::createEmptyGameObject()
{
	GameObject* temp = new GameObject();
	sceneObjects.push_back(temp);
	return temp;
}

GameObject * Scene::createEmptyGameObject(DirectX::XMVECTOR position)
{
	GameObject* temp = new GameObject(position);
	sceneObjects.push_back(temp);
	return temp;
}

GameObject * Scene::getGameObjectAt(int index)
{
	return sceneObjects.at(index);
}

std::vector<GameObject*> Scene::getFrustumCulledResult()
{
	return frustumCulledResult;
}

std::vector<GameObject*> Scene::getSceneObjects() {
	return sceneObjects;
}

int Scene::getSceneObjectsCount()
{
	return sceneObjects.size();
}



void Scene::update()
{
	for (int i = 0; i < sceneObjects.size(); i++) {
		for (int j = 0; j < sceneObjects[i]->components.size(); j++) {
			sceneObjects[i]->components[j]->update();
		}
	}
}

void Scene::SaveScene()
{
	using json = nlohmann::json;


	std::vector<json> jason;

	for (size_t i = 0; i < getSceneObjectsCount(); i++)
	{
		json j;
		GameObject* temp = getSceneObjects().at(i);
		JsonSceneWriter write = JsonSceneWriter(temp->name, Vector3(temp->transform.getPosition()), Vector3(temp->transform.getRotation()));
		j["name"] = write.name;
		j["position"] = write.position.toString();
		j["rotation"] = write.rotation.toString();

		for (size_t k = 0; k < temp->components.size(); ++k)
		{
			std::string compName = temp->components.at(k)->assetName;

			if (compName == "Camera")
			{
				Camera *c = temp->getComponent<Camera>();
				j["FOV"] = c->FOV;
				j["Near_Z"] = c->nearZ;
				j["Far_Z"] = c->farZ;
				j["AssetName"] = c->assetName;
			}
			else if (compName == "Material")
			{
				Material* m = temp->getComponent<Material>();
			}
		}

		jason.push_back(j);
	}

	json end = {
		{
			"Scene-Name",
			{
				jason
			}
		}

	};

	std::ofstream o("scene.aur");
	o << std::setw(4) << end << std::endl;
}

void Scene::LoadScene()
{
	//ProgressReport::Get().Reset(g_progress_Scene);
	//ProgressReport::Get().SetStatus(g_progress_Scene, "Saving scene...");
	//Stopwatch timer;

	//// Add scene file extension to the filepath if it's missing
	//string filePath = filePathIn;
	//if (FileSystem::GetExtensionFromFilePath(filePath) != SCENE_EXTENSION)
	//{
	//	filePath += SCENE_EXTENSION;
	//}

	//// Save any in-memory changes done to resources while running.
	//m_context->GetSubsystem<ResourceManager>()->SaveResourcesToFiles();

	//// Create a prefab file
	//auto file = make_unique<FileStream>(filePath, FileStreamMode_Write);
	//if (!file->IsOpen())
	//	return false;

	//// Save currently loaded resource paths
	//vector<string> filePaths;
	//m_context->GetSubsystem<ResourceManager>()->GetResourceFilePaths(filePaths);
	//file->Write(filePaths);

	////= Save GameObjects ============================
	//// Only save root GameObjects as they will also save their descendants
	//vector<weak_ptr<GameObject>> rootGameObjects = GetRootGameObjects();

	//// 1st - GameObject count
	//int rootGameObjectCount = (int)rootGameObjects.size();
	//file->Write(rootGameObjectCount);

	//// 2nd - GameObject IDs
	//for (const auto& root : rootGameObjects)
	//{
	//	file->Write(root.lock()->GetID());
	//}

	//// 3rd - GameObjects
	//for (const auto& root : rootGameObjects)
	//{
	//	root.lock()->Serialize(file.get());
	//}
	////==============================================

	//LOG_INFO("Scene: Saving took " + to_string((int)timer.GetElapsedTimeMs()) + " ms");
	//FIRE_EVENT(EVENT_SCENE_SAVED);

	//ProgressReport::Get().SetIsLoading(g_progress_Scene, false);

	//return true;
}

std::vector<GameObject*> Scene::frustumCull(GameObject * camera)
{
	std::vector<GameObject*> objectsToRender;

	//Create planes


	PLANE planes[6];

	DirectX::XMMATRIX matrix = DirectX::XMMatrixMultiply(camera->getComponent<Camera>()->calculateViewMatrix(), camera->getComponent<Camera>()->calculatePerspectiveMatrix());
	DirectX::XMFLOAT4X4 m;
	DirectX::XMStoreFloat4x4(&m, matrix);

	//Left
	planes[0].normal.x = -(m._14 + m._11);
	planes[0].normal.y = -(m._24 + m._21);
	planes[0].normal.z = -(m._34 + m._31);
	planes[0].distance = -(m._44 + m._41);
	//Right
	planes[1].normal.x = -(m._14 - m._11);
	planes[1].normal.y = -(m._24 - m._21);
	planes[1].normal.z = -(m._34 - m._31);
	planes[1].distance = -(m._44 - m._41);

	//Top
	planes[2].normal.x = -(m._14 + m._12);
	planes[2].normal.y = -(m._24 + m._22);
	planes[2].normal.z = -(m._34 + m._32);
	planes[2].distance = -(m._44 + m._42);
	//Bottom
	planes[3].normal.x = -(m._14 - m._12);
	planes[3].normal.y = -(m._24 - m._22);
	planes[3].normal.z = -(m._34 - m._32);
	planes[3].distance = -(m._44 - m._42);

	//Near
	planes[4].normal.x = -(m._14 + m._13);
	planes[4].normal.y = -(m._24 + m._23);
	planes[4].normal.z = -(m._34 + m._33);
	planes[4].distance = -(m._44 + m._43);
	//Far
	planes[5].normal.x = -(m._14 - m._13);
	planes[5].normal.y = -(m._24 - m._23);
	planes[5].normal.z = -(m._34 - m._33);
	planes[5].distance = -(m._44 - m._43);

	for (int i = 0; i < 6; i++)
	{
		float denom = 1.0f / sqrt(pow(planes[i].normal.x, 2) + pow(planes[i].normal.y, 2) + pow(planes[i].normal.z, 2));
		planes[i].normal.x *= denom;
		planes[i].normal.y *= denom;
		planes[i].normal.z *= denom;
		planes[i].distance *= denom;

	}


	//Result:
	//0 outside
	//1 inside
	//2 intersecting
	for (int i = 0; i < sceneObjects.size(); i++) {
		if (sceneObjects[i]->getIsRenderable()) {
			int cullingResult = frustumCheck(sceneObjects[i]->getComponent<MeshFilter>()->getBoundingBox(), sceneObjects[i]->transform.getPosition(), planes);

			//print culling result
			//sceneObjects[i]->name = std::to_string(cullingResult);

			if (cullingResult > 0) {
				objectsToRender.push_back(sceneObjects[i]);
			}
		}
	}

	frustumCulledResult = objectsToRender;
	return objectsToRender;
}

template <typename T> T convert_to(const std::string &str)
{
	std::istringstream ss(str);
	T num;
	ss >> num;
	return num;
}

int Scene::frustumCheck(OOBB otherOBB, DirectX::XMVECTOR otherPosition, PLANE *planes)
{

	int intersecting = 0;
	for (int j = 0; j < 6; j++) {
		int result = planeAABBIntersect(otherOBB, otherPosition, DirectX::XMVectorSet(planes[j].normal.x, planes[j].normal.y, planes[j].normal.z, planes[j].distance));
		if (result == 0) {
			return 0;
			break;
		}

		else if (result == 2) {
			intersecting = 1;
		}

	}
	if (intersecting == 1) {
		return 2;
	}
	else {
		return 1;
	}
}

int Scene::planeAABBIntersect(OOBB otherOBB, DirectX::XMVECTOR otherPosition, DirectX::XMVECTOR frustumPlane)
{

	DirectX::XMVECTOR diagonals[4];
	diagonals[0] = DirectX::XMVectorSubtract(otherOBB.corners[0], otherOBB.corners[1]);
	diagonals[1] = DirectX::XMVectorSubtract(otherOBB.corners[2], otherOBB.corners[3]);
	diagonals[2] = DirectX::XMVectorSubtract(otherOBB.corners[4], otherOBB.corners[5]);
	diagonals[3] = DirectX::XMVectorSubtract(otherOBB.corners[6], otherOBB.corners[7]);

	float lengthV = 0;
	for (int i = 0; i < 4; i++) {
		float len = abs(DirectX::XMVectorGetW(DirectX::XMVector3Dot(diagonals[i], frustumPlane)));
		if (len > lengthV) {
			lengthV = len;
		}
	}



	float e = lengthV / 2;
	float s = DirectX::XMVectorGetW(DirectX::XMVector3Dot((DirectX::XMVectorAdd(otherOBB.centre, otherPosition)), frustumPlane)) + DirectX::XMVectorGetW(frustumPlane);//+d plane equation

	if ((s - e) > 0) {
		return 0;
	}
	if ((s + e) < 0) {
		return 1;
	}
	return 2;
}
