
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
	void setMesh(Mesh* in_setMesh);
	OOBB getBoundingBox() { return boundingBox; };

	void update();
private:
	OOBB boundingBox;
	Mesh * mesh = nullptr;
	std::vector<DirectX::XMVECTOR>* vertexPositions;

};
