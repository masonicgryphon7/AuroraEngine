#pragma once
#include "PRIMITIVE_GEOMETRY.h"
#include"Scene.h"
#define EPSILON 0.00001

static class Physics
{
public:
	Physics();

	~Physics();

	bool Raycast(Ray ray, RaycastHit& hit);
	std::vector<GameObject*> ScreenSelection(DirectX::XMVECTOR startXYendXY, GameObject* camera);

private:
	float obbTest(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin, OOBB o);
	float triangleTest(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR gameObjectPosition,std::vector<DirectX::XMVECTOR>* vertexPositions);


}gPhysics;

