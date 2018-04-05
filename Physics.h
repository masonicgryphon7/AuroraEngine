#pragma once
#include "PRIMITIVE_GEOMETRY.h"
#include"Scene.h"

static class Physics
{
public:
	Physics();
	//Physics(Scene* scene);

	~Physics();

	bool Raycast(Ray ray, RaycastHit& hit);

private:
	Scene* scene;


}Physics;

