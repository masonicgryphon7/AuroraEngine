#pragma once
#include "PRIMITIVE_GEOMETRY.h"
#include"Scene.h"
#define EPSILON 0.0001

static class Physics
{
public:
	Physics();
	Physics(Scene* scene);

	~Physics();

	bool Raycast(Ray ray, RaycastHit& hit);

private:
	Scene* scene;
	float obbTest(DirectX::XMVECTOR rayDir, DirectX::XMVECTOR rayOrigin, OOBB o);

}Physics;

