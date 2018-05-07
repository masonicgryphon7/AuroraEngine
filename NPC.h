#pragma once
#include "Component.h"
#include <vector>
#include "Unit.h"
#include "Mesh.h"
#include "MeshFilter.h"
#include "Scene.h"
#include "AssetManager.h"
#include "Debug.h"
class NPC : public Component
{
public:
	NPC(std::vector<Unit*> *player_units, std::vector<Unit*> *player_buildings);
	~NPC();
	void update();
	void instantiate_NPC();
	

private:
	std::vector<Unit*> *player_units;
	std::vector<Unit*> *player_buildings;

	std::vector<Unit*> npc_units;

	bool wantsToAttackHero;

	void gather();
	void findNewSource();
	void findNewEnemies(Unit* unit);
	void attack(Unit* unit);
	void standAbout(Unit* unit);
	void summonWorker(Unit* building);

	

};

