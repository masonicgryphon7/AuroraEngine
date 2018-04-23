#ifndef UNIT_H
#define UNIT_H

#include"Component.h"
#include <DirectXMath.h>
#include "Transform.h"
#include <vector>
#include "PRIMITIVE_GEOMETRY.h"

#pragma once
	/*
	Factions
	Stats
	Orders
	Resources
	Type
	*/
	struct Stats
	{
		int HealthPoints;
		int AttackPoints;
		int DefencePoints;
	};
	
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
		float distance;
		Command command;
	};

class Unit : public Component
{
private:

	int Resources;
	Type type;
	std::vector<Order> UnitOrders;
	Stats UnitStats;

public:

	Unit();
	~Unit();

	Type getType() { return this->type; };
	Stats getStats() { return this->UnitStats; };
	std::vector<Order> getUnitOrders() { return this->UnitOrders; };

	void MoveCommand();

	void RecieveOrder(RaycastHit Values);
	void update();

};

#endif


