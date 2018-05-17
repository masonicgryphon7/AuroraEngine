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
	
	bool wantsToAttackHero;

	int nrOfSoldiers = 0;
	int nrOfWorkers = 0;

	void gather(Unit* unitToUse);
	void findNewSource();
	void findNewEnemies(Unit* unit);
	void attack(Unit* unit);
	void standAbout(Unit* unit);
	void summonWorker(Unit* building);
	void takeOverBuilding(Unit* unitToUse);
	
};

