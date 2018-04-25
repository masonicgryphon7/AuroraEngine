#ifndef UNIT_H
#define UNIT_H

#include"Component.h"
#include <DirectXMath.h>
#include "Transform.h"
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"
#include "Node.h"

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
		Build,
		Follow,
		Summon,
		Idle
	};

	enum Type
	{
		Hero,
		Soldier,
		Worker,
		Building,
		//BrainParasite,
		//Nature
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

	float distance;
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


	int getHealthPoints() { return this->healthPoints; };
	int getAttackPoints() { return this->attackPoints; };
	int getDefencePoints() { return this->defencePoints; };
	float getAttackDistance() { return this->attackDistance; };

	void setHealthPoints(int healthPoints) { this->healthPoints = healthPoints; };
	void setAttackPoints(int attackPoints) { this->attackPoints = attackPoints; };
	void setDefencePoints(int defencePoints) { this->defencePoints = defencePoints; };
	void setAttackDistance(float attackDistance) { this->attackDistance = attackDistance; };

	void MoveCommand();
	void attackCommand();

	void RecieveOrder(RaycastHit Values);
	void update();

};

#endif


