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
	bool tempBool = false;

	DirectX::XMVECTOR unitPos;
	float findClosest = 10000;

	std::clock_t start;
	float waitTime;
	bool checkTime;

	int harassUnits = 0;
	
	bool wantsToAttackHero;


	void gather(Unit* unitToUse);
	void findNewEnemies(Unit* unit);
	void attack(Unit* unit);
	void standAbout(Unit* unit);
	void summonUnit(Unit* building);
	void takeOverBuilding(Unit* unitToUse);
	
};

