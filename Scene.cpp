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

std::vector<GameObject*> Scene::getObjectsToRender(GameObject * camera)
{
	return frustumCull(camera);
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


std::vector<GameObject*> Scene::frustumCull(GameObject * camera)
{
	std::vector<GameObject*> objectsToRender;

	//Create planes
	struct PLANE
	{
		DirectX::XMFLOAT3 normal;
		float distance;
	};

	PLANE planes[6];

	DirectX::XMMATRIX matrix = DirectX::XMMatrixMultiply(camera->getComponent<Camera>()->calculateViewMatrix(camera->transform.getPosition(), camera->transform.getForward(), camera->transform.getUp()), camera->getComponent<Camera>()->calculatePerspectiveMatrix());
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
			int result = 0;

			int intersecting = 0;
			for (int j = 0; j<6; j++) {
				int result = planeAABBIntersect(sceneObjects[i]->OOBoundingBox, sceneObjects[i]->transform.getPosition(), DirectX::XMVectorSet(planes[j].normal.x, planes[j].normal.y, planes[j].normal.z, planes[j].distance));
				if (result == 0) {
					result = 0;
				}
				else if (result == 2) {
					intersecting = 1;
				}

			}
			if (intersecting == 1) {
				result = 2;
			}
			else {
				result = 1;
			}


			if (result > 0) {
				objectsToRender.push_back(sceneObjects[i]);
			}
		}
	}

	return objectsToRender;
}

int Scene::planeAABBIntersect(OOBB otherOBB, DirectX::XMVECTOR otherPosition, DirectX::XMVECTOR frustumPlane)
{
	DirectX::XMVECTOR corners[8];

	corners[0] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						otherOBB.x_hx,
						DirectX::XMVectorGetW(otherOBB.x_hx)),
					DirectX::XMVectorScale(
						otherOBB.y_hy,
						DirectX::XMVectorGetW(otherOBB.y_hy))),
				DirectX::XMVectorScale(
					otherOBB.z_hz,
					DirectX::XMVectorGetW(otherOBB.z_hz))),
			DirectX::XMVectorAdd(
				otherOBB.centre,
				otherPosition));

	corners[1] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						otherOBB.x_hx,
						-1.0f*DirectX::XMVectorGetW(otherOBB.x_hx)),
					DirectX::XMVectorScale(
						otherOBB.y_hy,
						-1.0f*DirectX::XMVectorGetW(otherOBB.y_hy))),
				DirectX::XMVectorScale(
					otherOBB.z_hz,
					-1.0f*DirectX::XMVectorGetW(otherOBB.z_hz))),
			DirectX::XMVectorAdd(
				otherOBB.centre,
				otherPosition));
	corners[2] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						otherOBB.x_hx,
						DirectX::XMVectorGetW(otherOBB.x_hx)),
					DirectX::XMVectorScale(
						otherOBB.y_hy,
						DirectX::XMVectorGetW(otherOBB.y_hy))),
				DirectX::XMVectorScale(
					otherOBB.z_hz,
					-1.0f*DirectX::XMVectorGetW(otherOBB.z_hz))),
			DirectX::XMVectorAdd(
				otherOBB.centre,
				otherPosition));
	corners[3] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						otherOBB.x_hx,
						-1.0f*DirectX::XMVectorGetW(otherOBB.x_hx)),
					DirectX::XMVectorScale(
						otherOBB.y_hy,
						-1.0f*DirectX::XMVectorGetW(otherOBB.y_hy))),
				DirectX::XMVectorScale(
					otherOBB.z_hz,
					DirectX::XMVectorGetW(otherOBB.z_hz))),
			DirectX::XMVectorAdd(
				otherOBB.centre,
				otherPosition));
	corners[4] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						otherOBB.x_hx,
						-1.0f*DirectX::XMVectorGetW(otherOBB.x_hx)),
					DirectX::XMVectorScale(
						otherOBB.y_hy,
						DirectX::XMVectorGetW(otherOBB.y_hy))),
				DirectX::XMVectorScale(
					otherOBB.z_hz,
					DirectX::XMVectorGetW(otherOBB.z_hz))),
			DirectX::XMVectorAdd(
				otherOBB.centre,
				otherPosition));
	corners[5] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						otherOBB.x_hx,
						DirectX::XMVectorGetW(otherOBB.x_hx)),
					DirectX::XMVectorScale(
						otherOBB.y_hy,
						-1.0f*DirectX::XMVectorGetW(otherOBB.y_hy))),
				DirectX::XMVectorScale(
					otherOBB.z_hz,
					-1.0f*DirectX::XMVectorGetW(otherOBB.z_hz))),
			DirectX::XMVectorAdd(
				otherOBB.centre,
				otherPosition));
	corners[6] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						otherOBB.x_hx,
						-1.0f*DirectX::XMVectorGetW(otherOBB.x_hx)),
					DirectX::XMVectorScale(
						otherOBB.y_hy,
						DirectX::XMVectorGetW(otherOBB.y_hy))),
				DirectX::XMVectorScale(
					otherOBB.z_hz,
					-1.0f*DirectX::XMVectorGetW(otherOBB.z_hz))),
			DirectX::XMVectorAdd(
				otherOBB.centre,
				otherPosition));
	corners[7] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						otherOBB.x_hx,
						DirectX::XMVectorGetW(otherOBB.x_hx)),
					DirectX::XMVectorScale(
						otherOBB.y_hy,
						-1.0f*DirectX::XMVectorGetW(otherOBB.y_hy))),
				DirectX::XMVectorScale(
					otherOBB.z_hz,
					DirectX::XMVectorGetW(otherOBB.z_hz))),
			DirectX::XMVectorAdd(
				otherOBB.centre,
				otherPosition));


	DirectX::XMVECTOR diagonals[4];
	diagonals[0] = DirectX::XMVectorSubtract(corners[0], corners[1]);
	diagonals[1] = DirectX::XMVectorSubtract(corners[2], corners[3]);
	diagonals[2] = DirectX::XMVectorSubtract(corners[4], corners[5]);
	diagonals[3] = DirectX::XMVectorSubtract(corners[6], corners[7]);

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
