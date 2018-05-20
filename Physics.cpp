#include "Physics.h"
#include "Debug.h"


Physics::Physics()
{
}




Physics::~Physics()
{
}

bool Physics::Raycast(Ray ray, RaycastHit & hit)
{
	bool hitObject = false;
	float lastT = -1;
	int objIndex = -1;


	//
	for (int i = 0; i < gScene.getSceneObjectsCount(); i++)
	{
		if (gScene.getSceneObjects()[i]->raycastOption != RayCastOptions::NONE) {
			//OBB
			if (gScene.getSceneObjects()[i]->getIsRenderable()) {
				OOBB obb = gScene.getSceneObjects()[i]->getComponent<MeshFilter>()->getBoundingBox();
				if (obb.isActive == true) {
					obb.centre = DirectX::XMVectorAdd(obb.centre, gScene.getSceneObjects()[i]->transform.getPosition());

					float t = obbTest(ray.direction, ray.origin, obb);
					if (t > EPSILON && (t < lastT || lastT < EPSILON)) {
				

						//Detail
						if (gScene.getSceneObjects()[i]->raycastOption == RayCastOptions::DETAILED) {
							//Debug.Log("Detailed");
							float t = triangleTest(ray.direction, ray.origin, gScene.getSceneObjects()[i]->transform.getPosition(), gScene.getSceneObjects()[i]->getComponent<MeshFilter>()->getMesh()->getVertexPositions());

							if (t > EPSILON && (t < lastT || lastT < EPSILON)) {
								lastT = t;
								objIndex = i;
								hitObject = true;
							}
						}
						else {
							lastT = t;
							objIndex = i;
							hitObject = true;

						}

					}
				}

			}
		}
		

	}

	if (hitObject) {
		hit.distance = lastT;
		hit.point = DirectX::XMVectorAdd(DirectX::XMVectorScale(ray.direction, lastT), ray.origin);
		hit.transform = &gScene.getSceneObjects()[objIndex]->transform;
	}
	else
	{
		hit.distance = -1;
		hit.point =DirectX::XMVectorSet(0,0,0,0);
		hit.transform = nullptr;
	}


	return hitObject;
}

std::vector<GameObject*> Physics::ScreenSelection(DirectX::XMVECTOR startXYendXY, GameObject * camera)
{
	std::vector<GameObject*> selectedGameObjects;

	bool selected = false;

	//input position to ndc
	//normalize to 0-1
	float width = camera->getComponent<Camera>()->width;
	float height = camera->getComponent<Camera>()->height;

	DirectX::XMVECTOR mouseStart = DirectX::XMVectorMultiply(startXYendXY, DirectX::XMVectorSet(1.0f / width, 1.0f / height, 0.0f, 0.0f));
	DirectX::XMVECTOR mouseEnd = DirectX::XMVectorMultiply(startXYendXY, DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f / width, 1.0f / height));

	//map to -1 1
	mouseStart = DirectX::XMVectorMultiply(mouseStart, DirectX::XMVectorSet(2.0f, -2.0f, 0.0f, 0.0f));
	mouseStart = DirectX::XMVectorSubtract(mouseStart, DirectX::XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f));

	//map to -1 1
	mouseEnd = DirectX::XMVectorMultiply(mouseEnd, DirectX::XMVectorSet(0.0f, 0.0f,2.0f, -2.0f));
	mouseEnd = DirectX::XMVectorSubtract(mouseEnd, DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, -1.0f ));


	float startX = DirectX::XMVectorGetX(mouseStart);
	float startY = DirectX::XMVectorGetY(mouseStart);
	float endX = DirectX::XMVectorGetZ(mouseEnd);
	float endY = DirectX::XMVectorGetW(mouseEnd);

	float minX = min(startX, endX);
	float maxX = max(startX, endX);
	float minY = min(startY, endY);
	float maxY = max(startY, endY);


	std::vector<GameObject*>* temp = gScene.getFrustumCulledResult();
	DirectX::XMVECTOR screenSpaceCorners[8];
	DirectX::XMMATRIX viewToScreenMatrix = DirectX::XMMatrixMultiply(camera->getComponent<Camera>()->calculateViewMatrix(), camera->getComponent<Camera>()->calculatePerspectiveMatrix());
	DirectX::XMFLOAT4X4 m;
	DirectX::XMStoreFloat4x4(&m, viewToScreenMatrix);
	float w = m._44;
	w = 1 / w;
	viewToScreenMatrix = viewToScreenMatrix * DirectX::XMMatrixScaling(w, w, w);

	for (int i = 0; i < temp->size(); i++)
	{
		if (temp[0][i]->getIsRenderable()) {

			OOBB obb = temp[0][i]->getComponent<MeshFilter>()->getBoundingBox();

			bool include = false;
			for (int j = 0; j < 8; j++)
			{
				screenSpaceCorners[j] = DirectX::XMVector3Transform(DirectX::XMVectorAdd(obb.corners[j], temp[0][i]->transform.getPosition()), viewToScreenMatrix);
				float x =DirectX::XMVectorGetX(screenSpaceCorners[j]);
				float y = DirectX::XMVectorGetY(screenSpaceCorners[j]);

				if (x <= maxX && x > minX) {
					if (y <= maxY && y > minY) {
						include = true;
						break;
					}
				}
			}

			if (include) {
				selectedGameObjects.push_back(temp[0][i]);
			}

		}
	}

	/*for (int i = 0; i < selectedGameObjects.size(); i++)
	{
		Debug.Log(selectedGameObjects[i]->name, "was selected");
	}*/

	return selectedGameObjects;
}


float Physics::obbTest(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin, OOBB o)
{
	// IMPLEMENT HERE
	float tMin = -INFINITE;
	float tMax = INFINITE;
	DirectX::XMVECTOR distanceToOBB =DirectX::XMVectorSubtract(o.centre, rayOrigin);
	DirectX::XMVECTOR sides[3];
	sides[0] = o.x_hx;
	sides[1] = o.y_hy;
	sides[2] = o.z_hz;

	for (int i = 0; i<3; i++) {
		float e = DirectX::XMVectorGetW(DirectX::XMVector3Dot(sides[i], distanceToOBB));
		float f = DirectX::XMVectorGetW(DirectX::XMVector3Dot(sides[i], rayDir));
		if (abs(f)>EPSILON) {
			float t1 = (e + DirectX::XMVectorGetW(sides[i])) / f;
			float t2 = (e - DirectX::XMVectorGetW( sides[i])) / f;

			if (t1>t2) {
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			if (t1>tMin) {
				tMin = t1;
			}

			if (t2<tMax) {
				tMax = t2;
			}

			if (tMin>tMax) {
				return -1;
			}

			if (tMax<EPSILON) {
				return -1;
			}
		}
		else if ((-e - DirectX::XMVectorGetW( sides[i])>EPSILON) || (-e + DirectX::XMVectorGetW(sides[i]))<EPSILON) {
			return -1;
		}
	}
	//////
	if (tMin>EPSILON) {
		return tMin;
	}
	else {
		return tMax;
	}




}
float Physics::triangleTest(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR gameObjectPosition, std::vector<DirectX::XMVECTOR>* vertexPositions)
{

	float lastT = -1;

	for (int i = 0; i < vertexPositions->size(); i = i + 3)
	{

		// IMPLEMENT HERE
		float t = -1;
		DirectX::XMVECTOR e1 = DirectX::XMVectorSubtract(vertexPositions[0][i + 1], vertexPositions[0][i]);
		DirectX::XMVECTOR e2 = DirectX::XMVectorSubtract(vertexPositions[0][i + 2], vertexPositions[0][i]);

		//make possible for rotation?????????????????
		DirectX::XMVECTOR s = DirectX::XMVectorSubtract(rayOrigin,  DirectX::XMVectorAdd(gameObjectPosition, vertexPositions[0][i]));
		DirectX::XMMATRIX matrix = DirectX::XMMATRIX(DirectX::XMVectorScale(rayDir, -1), e1, e2, DirectX::XMVectorSet(0, 0, 0, 1));
		float denominator = DirectX::XMVectorGetW(DirectX::XMMatrixDeterminant(matrix));
		if (denominator > EPSILON) {
			float det1 = DirectX::XMVectorGetW(DirectX::XMMatrixDeterminant(DirectX::XMMATRIX(s, e1, e2, DirectX::XMVectorSet(0, 0, 0, 1))));
			float det2 = DirectX::XMVectorGetW(DirectX::XMMatrixDeterminant(DirectX::XMMATRIX(DirectX::XMVectorScale(rayDir, -1), s, e2, DirectX::XMVectorSet(0, 0, 0, 1))));
			float det3 = DirectX::XMVectorGetW(DirectX::XMMatrixDeterminant(DirectX::XMMATRIX(DirectX::XMVectorScale(rayDir, -1), e1, s, DirectX::XMVectorSet(0, 0, 0, 1))));
			DirectX::XMVECTOR tuv = DirectX::XMVectorSet(det1, det2, det3, 0);
			tuv = DirectX::XMVectorScale(tuv, (1 / denominator));
			float w = 1 - DirectX::XMVectorGetY(tuv) - DirectX::XMVectorGetZ(tuv);

			if (DirectX::XMVectorGetY(tuv) >= EPSILON && DirectX::XMVectorGetY(tuv) <= 1 && DirectX::XMVectorGetZ(tuv) >= EPSILON && DirectX::XMVectorGetZ(tuv) <= 1 && w >= EPSILON && w <= 1) {
				t = DirectX::XMVectorGetX(tuv);
			}
		}

		if (t > EPSILON && (t < lastT || lastT < EPSILON)) {
			lastT = t;

		}


	}
		return lastT;
}
