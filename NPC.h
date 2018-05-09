#pragma once
#include "Component.h"
#include <vector>
#include "Unit.h"
#include "Mesh.h"
#include "MeshFilter.h"
#include "Scene.h"
#include "AssetManager.h"
#include "Debug.h"
class Unit;
class NPC : public Component
{
public:
	NPC();
	~NPC();
	void update();
	void instantiate_NPC();
	

private:
	//std::vector<Unit*> npc_units;
	//std::vector<Unit*> npc_buildings;

	bool wantsToAttackHero;

	void gather();
	void findNewSource();
	void findNewEnemies(Unit* unit);
	void attack(Unit* unit);
	void standAbout(Unit* unit);
	void summonWorker(Unit* building);

	

};

