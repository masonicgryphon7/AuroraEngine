#include "Scene.h"
#include "Debug.h"
#include "AssetManager.h"

std::vector<GameObject*> Scene::frustumCulledResult;
std::vector<GameObject*> Scene::sceneObjects;
GameObject* Scene::selectedGameObject;

Scene::Scene()
{
}


Scene::~Scene()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (sceneObjects[i])
			delete sceneObjects[i];
		sceneObjects[i] = nullptr;
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


GameObject* Scene::CreateGameObject(Primitives primitive, Vector3 position, Vector3 rotation)
{
	GameObject* temp = nullptr;
	switch (primitive)
	{
	case Primitives::Empty:
		temp = new GameObject(position.asXMVECTOR());
		temp->transform.setRotation(rotation.asXMVECTOR());
		break;

	case Primitives::Cube:
		Debug.Log("Name; ", AssetManager.getMesh("Cube"));
		break;
	}

	sceneObjects.push_back(temp);
	selectedGameObject = temp;
	//Debug.Log("\tPosition: ", position.toString(), "\tRotation: ", rotation.toString());
	return temp;
}

GameObject* Scene::CreateGameObject(Mesh * mesh, Vector3 position, Vector3 rotation)
{
	GameObject* temp = new GameObject(position.asXMVECTOR());
	temp->name = mesh->getMeshName();
	MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh(0));
	temp->addComponent(new MaterialFilter(AssetManager.getMaterial(0)));
	temp->addComponent(meshFilter);
	sceneObjects.push_back(temp);
	return temp;
}

GameObject * Scene::getGameObjectAt(int index)
{
	return sceneObjects.at(index);
}

std::vector<GameObject*>* Scene::getFrustumCulledResult()
{
	return &frustumCulledResult;
}

std::vector<GameObject*> Scene::getSceneObjects() {
	return sceneObjects;
}

int Scene::getSceneObjectsCount()
{
	return sceneObjects.size();
}

void Scene::destroyGameObjects()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (sceneObjects[i]->destroyGO==true) {
			delete sceneObjects[i];
			sceneObjects.erase(sceneObjects.begin() + i);

		}
	}
}



void Scene::update()
{
	for (int i = 0; i < sceneObjects.size(); i++) {
		for (int j = 0; j < sceneObjects[i]->components.size(); j++) {
			sceneObjects.at(i)->components.at(j)->update();
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
		std::string meshPath = "NULL";
		if (temp->meshFilterComponent != nullptr)
			meshPath = temp->meshFilterComponent->getMesh()->getMeshPath();

		JsonSceneWriter write = JsonSceneWriter(temp->name, meshPath, Vector3(temp->transform.getPosition()), Vector3(temp->transform.getRotation()));
		j["name"] = write.name;

		if (meshPath != "NULL")
			j["meshPath"] = write.filePath;

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
				Material* m = temp->getComponent<MaterialFilter>()->material;
			}
		}

		jason.push_back(j);
	}

	json end =
	{
		{
			"Scene-Name",
			{
				jason
			}
		}
	};

	std::string encryptedSave = Encrypt(end.dump(4), "dSXOv9vbM1MIm5kAf4yjjw");
	//Debug.Log("Encrypted: ", encryptedSave);
	//Debug.Log("Decrypted: ", Decrypt(encryptedSave, "dSXOv9vbM1MIm5kAf4yjjw"));

	std::ofstream o("scene.aur");
	std::ofstream os("scene_u.aur");
	o << std::setw(4) << encryptedSave << std::endl;
	os << std::setw(4) << end << std::endl;
}

void Scene::LoadScene()
{
	std::ifstream i("scene.aur");

	std::string takeIn((std::istreambuf_iterator<char>(i)),
		std::istreambuf_iterator<char>()), decrypted;

	decrypted = Decrypt(takeIn, "dSXOv9vbM1MIm5kAf4yjjw");

	std::istringstream f(decrypted);
	std::string line;

	GameObject* tempGameObject = nullptr;

	// -- STUFF TO LOAD IN -- //

	std::string meshPath = "", objectName = "";
	Vector3 position = Vector3(0, 0, 0), rotation = Vector3(0, 0, 0);

	//while (std::getline(f, line))
	//{
	//	meshPath = SetMeshPath(meshPath, line);
	//	objectName = SetObjectName(objectName, line);
	//	position = SetPosition(position, line);
	//	rotation = SetRotation(rotation, line);

	//	if (objectName == "Editor Camera")
	//	{
	//		meshPath = "";
	//		objectName = "";
	//		position = Vector3(0, 0, 0);
	//		rotation = Vector3(0, 0, 0);
	//		continue;
	//	}

	//	if (line.find("}") != std::string::npos)
	//	{
	//		tempGameObject = new GameObject(position.asXMVECTOR());
	//		tempGameObject->transform.setRotation(rotation.asXMVECTOR());
	//		tempGameObject->name = objectName;

	//		if (meshPath != "") 
	//		{
	//			Mesh* t_mesh = nullptr;
	//			MeshFilter* t_meshFilter = nullptr;
	//			MaterialFilter* t_materialFilter = nullptr;

	//			// add a way to actually know if this texture is for THIS material etc. etc. 

	//			AssetManager.AddTexture("Assets/STSP_ShadowTeam_BaseColor.png");
	//			AssetManager.AddTexture("Assets/STSP_ShadowTeam_Normal.png");
	//			AssetManager.AddTexture("Assets/STSP_ShadowTeam_OcclusionRoughnessMetallic.png");

	//			Material* t_material = AssetManager.AddMaterial(AssetManager.getShaderProgram(0));

	//			t_material->setAlbedo(AssetManager.getTexture(0)->getTexture());
	//			t_material->setNormal(AssetManager.getTexture(1)->getTexture());
	//			t_material->setAORoughMet(AssetManager.getTexture(2)->getTexture());

	//			t_mesh = AssetManager.AddMesh(meshPath);
	//			t_meshFilter = new MeshFilter(t_mesh);

	//			tempGameObject->addComponent(new MaterialFilter(t_material));
	//			tempGameObject->addComponent(t_meshFilter);
	//		}

	//		tempGameObject->transform.setPosition(position.asXMVECTOR());
	//		tempGameObject->transform.setRotation(rotation.asXMVECTOR());

	//		sceneObjects.push_back(tempGameObject);

	//		meshPath = "";
	//		objectName = "";
	//		position = Vector3(0, 0, 0);
	//		rotation = Vector3(0, 0, 0);
	//		//Debug.Log("RESET");
	//	}
	//}
}

bool Scene::ContainsGUID(std::string guid)
{
	bool hasGUID = false;
	for (std::string& g : containedGUID)
	{
		if (g == guid)
		{
			hasGUID = true; 
			break;
		}
	}

	if (!hasGUID)
		containedGUID.push_back(guid);

	return hasGUID;
}

std::string Scene::SetMeshPath(std::string str, const std::string & line)
{
	if (line.find("meshPath") == std::string::npos) return str;

	return Truncate(line);
}

std::string Scene::SetObjectName(std::string str, const std::string & line)
{
	if (line.find("name") == std::string::npos) return str;

	return Truncate(line);
}

Vector3 Scene::SetPosition(Vector3 vec, const std::string & line)
{
	if (line.find("position") == std::string::npos) return vec;

	std::string trunc = Truncate(line);

	std::string x = TruncateVector(trunc, 0), y = TruncateVector(trunc, 1), z = TruncateVector(trunc, 2);

	return vec;
}

Vector3 Scene::SetRotation(Vector3 vec, const std::string & line)
{
	if (line.find("rotation") == std::string::npos) return vec;

	std::string trunc = Truncate(line);

	std::string x = TruncateVector(trunc, 0), y = TruncateVector(trunc, 1), z = TruncateVector(trunc, 2);

	return vec;
}

std::string Scene::Truncate(std::string line)
{
	std::string temp;
	std::reverse(line.begin(), line.end());
	temp = line.substr(0, line.find(":", 0) - 2);
	std::reverse(temp.begin(), temp.end());
	temp = temp.substr(0, temp.length() - 2);
	return temp;
}

std::string Scene::TruncateVector(std::string line, unsigned int type)
{
	std::string justVecs;

	std::reverse(line.begin(), line.end());

	justVecs = line.substr(0, line.find("(", 0));

	std::reverse(justVecs.begin(), justVecs.end());

	justVecs = justVecs.substr(0, justVecs.length() - 1);

	if (type == 0) // X
	{
		justVecs = justVecs.substr(0, justVecs.find(",", 0));
	}
	else if (type == 1) // Y
	{
		justVecs = justVecs.substr(justVecs.find_first_of(" \t") + 1);
		justVecs = justVecs.substr(0, justVecs.find(",", 0));
	}
	else if (type == 2) // Z
	{
		std::reverse(justVecs.begin(), justVecs.end());
		justVecs = justVecs.substr(0, justVecs.find(",", 0));
		std::reverse(justVecs.begin(), justVecs.end());
	}

	return justVecs;
}

std::string Scene::Encrypt(std::string msg, std::string const & key)
{
	if (!key.size())
		return msg;

	for (std::string::size_type i = 0; i < msg.size(); ++i)
		msg[i] ^= key[i%key.size()];
	return msg;
}

std::string Scene::Decrypt(std::string const & msg, std::string const & key)
{
	return Encrypt(msg, key);
}

std::string m_StrCompressed;

std::string Scene::Compress(std::string str)
{
	std::string returnValue;
	int count = 1;
	bool canBeCompressed = false;

	int i = 1;
	while (i <= str.size())
	{
		if (str[i - 1] == str[i])
		{
			count++;
			if (count > 1)
				canBeCompressed = true;
		}
		else
		{
			returnValue += str[i - 1];
			returnValue += IntToString(count);
			count = 1;
		}
		i++;
	}

	if (canBeCompressed)
		return returnValue;
	else
		return str;
}

std::string Scene::IntToString(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

void Scene::frustumCull(GameObject * camera)
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
