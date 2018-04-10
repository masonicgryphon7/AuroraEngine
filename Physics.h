#pragma once
#include "PRIMITIVE_GEOMETRY.h"
#include"Scene.h"
#define EPSILON 0.0001

static class Physics
{
public:
	Physics();

	~Physics();

	bool Raycast(Ray ray, RaycastHit& hit);

private:
	float obbTest(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin, OOBB o);

}gPhysics;

