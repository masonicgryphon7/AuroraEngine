#pragma once
#include "Component.h"
#include "Mesh.h"

class MeshFilter : public Component
{
public:
	MeshFilter();
	MeshFilter(Mesh* mesh);

	~MeshFilter();

	Mesh* getMesh() { return mesh; };
	void setMesh(Mesh* in_setMesh) { mesh = in_setMesh; };

private:
	Mesh* mesh=nullptr;
};

