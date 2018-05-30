#ifndef UNIT_H
#define UNIT_H

#include"Component.h"
#include <DirectXMath.h>
#include "Transform.h"
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include "Node.h"
#include "GameManager.h"
class PlayerScript;
#pragma once
/*
Factions
Stats
Orders
Resources
Type
*/

enum Command
{
	Move = 1,
	Attack = 2,
	Gather = 3,
	HeroGather = 4,
	Drop = 5,
	SummonWorker = 6,
	SummonSoldier = 7,
	convertToSoldier = 8,
	takeBuilding = 9,
	Die = 10,
	Idle = 11
};

enum Type
{
	Hero,
	Soldier,
	Worker,
	Barrack,
	Bank,
	GoldMine
};

struct Order
{
	Transform* transform;
	DirectX::XMVECTOR point;
	Command command;
};

class Unit : public Component
{
private:

	int Resources;
	Type type;
	std::vector<Order> UnitOrders;
	int healthPoints, maxHealthPoints;
	int attackPoints;
	int defencePoints;
	float attackDistance;
	float actionTime = 0;
	float dieTime;
	int soundAction = 0;
	Transform* homePos;
	Transform* minePos;

	float findClosest = 10000;

	int e = 0;

	DirectX::XMVECTOR targetPos;
	DirectX::XMVECTOR unitPos;
	DirectX::XMVECTOR previousPos;


	PlayerScript* playerScript;

	float distance = 0;

	DirectX::XMFLOAT3 goalPos;
	DirectX::XMVECTOR offset;
	float lerpValue;
	std::vector<Node> pathNodes;
	std::vector<Node> worker_return_path;
	bool worker_has_path;
	int count = 0, count2 = 0, count3 = 0;
	bool hasReached = false;
	std::clock_t start;
	bool gatherCollect = false;
	DirectX::XMVECTOR lerpedPos;
public:


	Unit();
	Unit(Type UnitTypeSet);
	~Unit();

	bool isCollectingGold;
	GameObject* goldMineCollecting = nullptr;

	Type getType() { return this->type; };
	std::vector<Order> getUnitOrders() { return this->UnitOrders; };

	Command getUnitCommand();

	void clearUnitOrder() { UnitOrders.erase(UnitOrders.begin()); };
	void setUnitOrder(Order newOrder);
	int getUnitOrdersSize() { return this->UnitOrders.size(); };

	int getSoundAction() { return this->soundAction; };
	int getResources() { return this->Resources; };
	int getHealthPoints() { return this->healthPoints; };
	int getMaxHealthPoints() { return this->maxHealthPoints; };
	int getAttackPoints() { return this->attackPoints; };
	int getDefencePoints() { return this->defencePoints; };
	float getAttackDistance() { return this->attackDistance; };
	Transform* getHomePos() { return this->homePos; };
	Transform getTargetPos() { return this->targetPos; };
	//Transform* getMinePos() { return this->minePos; };

	float getUnitDistance() { return this->distance; };
	void setDistance(float newDistance) { this->distance = newDistance; };
	void setTargetPos(DirectX::XMVECTOR newTarget) { this->targetPos = newTarget; };

	int setSoundAction(int soundAction) { this->soundAction = soundAction; };
	void setResources(int resources) { this->Resources = resources; };
	void setHealthPoints(int healthPoints) { this->healthPoints = healthPoints; };
	void setAttackPoints(int attackPoints) { this->attackPoints = attackPoints; };
	void setDefencePoints(int defencePoints) { this->defencePoints = defencePoints; };
	void setAttackDistance(float attackDistance) { this->attackDistance = attackDistance; };
	void setHomePos(Transform* homePos) { this->homePos = homePos; };
	void setPlayerScript(PlayerScript* playerScript) { this->playerScript = playerScript; };
	void MoveCommand(DirectX::XMVECTOR *goalPos);
	void SecondMoveCommand(DirectX::XMVECTOR *goalPos);
	void attackCommand(Unit* targetedUnit);
	void attackEnemy();
	void takeDamage(int attackPoints, Unit* attackedBy);
	void takeFireDamage(float attackPoints);
	void FollowCommand();
	void gatherCommand(Unit* targetedUnit);
	void HeroGatherCommand(Unit* targetedUnit);
	void gatherResources();
	void dropCommand(Unit* targetedUnit);
	void dropResources(Transform* dropPos);
	void destroyUnit();
	void summonWorkerCommand();
	void convertToSoldierCommand(Unit* targetedUnit);
	void summonSoldierCommand();
	void takeBuildingCommand(Unit* targetedUnit);
	void dieCommand();
	std::vector<Order>* getUnitOrdersPointer() { return &UnitOrders; };
	float getDistanceBetweenUnits(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos);
	DirectX::XMVECTOR calculateOffsetInPath(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos);

	void ReceiveOrder(RaycastHit Values, int unitTag);
	void ReceiveOrder(OPTIONS option);
	void update();
};

#endif


