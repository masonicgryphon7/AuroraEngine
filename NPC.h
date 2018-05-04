#pragma once
#include "Component.h"
#include <vector>
#include "Unit.h"
#include "Mesh.h"
#include "MeshFilter.h"
#include "Scene.h"
#include "AssetManager.h"

class NPC : public Component
{
public:
	NPC();
	~NPC();
	void update();
	void instantiate_NPC();
	
private:
	
	std::vector<Unit> enemy_units;


};

