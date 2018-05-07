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
	Move,
	Attack,
	Gather,
	HeroGather,
	Drop,
	Build,
	Follow,
	SummonWorker,
	SummonSoldier,
	Idle
};

enum Type
{
	Hero,
	Soldier,
	Worker,
	Building,
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
	int healthPoints;
	int attackPoints;
	int defencePoints;
	float attackDistance;
	float actionTime;
	Transform* homePos;

	int e = 0;

	DirectX::XMVECTOR targetPos;
	DirectX::XMVECTOR unitPos;


	PlayerScript* playerScript;

	float distance = 0;

	DirectX::XMFLOAT3 goalPos;
	float lerpValue;
	std::vector<Node> pathNodes;
	int count = 0, count2 = 0, count3 = 0;

public:


	Unit();
	Unit(Type UnitTypeSet);
	~Unit();

	Type getType() { return this->type; };
	std::vector<Order> getUnitOrders() { return this->UnitOrders; };

	Command getUnitCommand() { return this->UnitOrders[0].command; };
	void clearUnitOrder() { this->UnitOrders.erase(UnitOrders.begin()); }

	int getUnitOrdersSize() { return this->UnitOrders.size(); };

	int getResources() { return this->Resources; };
	int getHealthPoints() { return this->healthPoints; };
	int getAttackPoints() { return this->attackPoints; };
	int getDefencePoints() { return this->defencePoints; };
	float getAttackDistance() { return this->attackDistance; };
	Transform* getHomePos() { return this->homePos; };
	Transform getTargetPos() { return this->targetPos; };
	float getUnitDistance() { return this->distance; };
	void setDistance(float newDistance) { this->distance = newDistance; };
	void setTargetPos(DirectX::XMVECTOR newTarget) { this->targetPos = newTarget; };


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
	void takeDamage(int attackPoints);
	void FollowCommand();
	void gatherCommand(Unit* targetedUnit);
	void HeroGatherCommand(Unit* targetedUnit);
	void gatherResources();
	void dropCommand(Unit* targetedUnit);
	void dropResources();
	void destroyUnit();
	void summonWorkerCommand();
	void summonSoldierCommand();
	std::vector<Order>* getUnitOrdersPointer() { return &UnitOrders; };
	float getDistanceBetweenUnits(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos);
	DirectX::XMVECTOR calculateOffsetInPath(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos);

	void ReceiveOrder(RaycastHit Values, int unitTag);
	void ReceiveOrder(OPTIONS option);
	void update();
};

#endif


