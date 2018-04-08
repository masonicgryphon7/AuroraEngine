#include "Physics.h"



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


	// OBBs
	for (int i = 0; i < gScene.getSceneObjectsCount(); i++)
	{
		OOBB obb = gScene.getSceneObjects()[i]->OOBoundingBox;
		if (obb.isActive == true) {
			obb.centre = DirectX::XMVectorAdd(obb.centre, gScene.getSceneObjects()[i]->transform.getPosition());

			float t = obbTest(ray.direction, ray.origin, obb);
			if (t>EPSILON && (t<lastT || lastT<EPSILON)) {
				lastT = t;
				objIndex = i;
				hitObject = true;

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


float Physics::obbTest(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin, OOBB o)
{
	// IMPLEMENT HERE
	float tMin = -10000000;
	float tMax = 10000000;
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




};
