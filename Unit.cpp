#include "Unit.h"
#include "GameObject.h"
#include "GUI_Viewport.h"
#include "InputHandler.h"
#include "GUI.h"
#include "Debug.h"
#include <DirectXMath.h>

Unit::Unit()
{
	actionTime = 2;

	switch (type)
	{
	case Type::Hero: //HERO
		this->healthPoints = 100;
		this->attackPoints = 13;
		this->defencePoints = 13;
		this->attackDistance = 1;
		this->Resources = 10;
		break;

	case Type::Soldier: //SOLDIER
		this->healthPoints = 20;
		this->attackPoints = 4;
		this->defencePoints = 8;
		this->attackDistance = 1;
		this->Resources = 0;
		break;

		case Type::Worker: //WORKER
		this->healthPoints = 15;
		this->attackPoints = 1;
		this->defencePoints = 5;
		this->attackDistance = 1;
		this->Resources = 0;
		break;

	case 3: //BUILDING
		this->healthPoints = 500;
		this->attackPoints = 0;
		this->defencePoints = 20;
		this->attackDistance = 0;
		this->Resources = 0;
		break;

	//case 4: //NATURE TREES, MINES, ETC

	//	this->UnitStats.HealthPoints = 100;
	//	this->UnitStats.AttackPoints = 0;
	//	this->UnitStats.DefencePoints = 0;
	//	this->Resources = 50;
	//	break;
	}
}

Unit::Unit(Type UnitTypeSet)
{
	actionTime = 2;

	switch (UnitTypeSet)
	{
	case Type::Hero: //HERO
		this->healthPoints = 100;
		this->attackPoints = 13;
		this->defencePoints = 13;
		this->attackDistance = 1;
		this->Resources = 10;
		this->type = Hero;
		break;

	case Type::Soldier: //SOLDIER
		this->healthPoints = 20;
		this->attackPoints = 4;
		this->defencePoints = 8;
		this->attackDistance = 1;
		this->Resources = 0;
		this->type = Soldier;
		break;

	case Type::Worker: //WORKER
		this->healthPoints = 15;
		this->attackPoints = 1;
		this->defencePoints = 5;
		this->attackDistance = 1;
		this->Resources = 0;
		this->type = Worker;
		break;

	case Type::Building: //BUILDING
		this->healthPoints = 500;
		this->attackPoints = 0;
		this->defencePoints = 20;
		this->attackDistance = 0;
		this->Resources = 0;
		this->type = Building;
		break;

		//case 4: //NATURE TREES, MINES, ETC

		//	this->UnitStats.HealthPoints = 100;
		//	this->UnitStats.AttackPoints = 0;
		//	this->UnitStats.DefencePoints = 0;
		//	this->Resources = 50;
		//	break;

	}
}

Unit::~Unit()
{

}

void Unit::MoveCommand()
{
	DirectX::XMFLOAT3 current;
	DirectX::XMStoreFloat3(&current, gameObject->transform.getPosition());

	DirectX::XMFLOAT3 pointPosition;
	DirectX::XMStoreFloat3(&pointPosition, UnitOrders.at(0).point);

	if (pathNodes.size() == 0)
		{
			lerpValue = 0;
			pathNodes = PathCreator.getPath(current, pointPosition); // Point position
		}

	if (pathNodes.size() > 0) {
		lerpValue += Time.getDeltaTime() * 10;
		if (lerpValue > 1) {
			lerpValue = 1;
		}
		DirectX::XMVECTOR goal = DirectX::XMVectorSet(pathNodes.at(0).position.x, pathNodes.at(0).position.y, pathNodes.at(0).position.z, 0);
		DirectX::XMFLOAT3 goalVec;
		DirectX::XMStoreFloat3(&goalVec, goal);


		if (DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract(goal, gameObject->transform.getPosition())))<EPSILON &&pathNodes.size() > 1) {
			pathNodes.erase(pathNodes.begin());

			goal = DirectX::XMVectorSet(pathNodes.at(0).position.x, pathNodes.at(0).position.y, pathNodes.at(0).position.z, 0);
			lerpValue = 0;
		}
		else if (DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract(goal, gameObject->transform.getPosition()))) < EPSILON &&pathNodes.size() == 1) {
			pathNodes.erase(pathNodes.begin());
			lerpValue = 0;
			UnitOrders.erase(UnitOrders.begin());


		}
		gameObject->transform.setPosition(DirectX::XMVectorLerp(gameObject->transform.getPosition(), goal, lerpValue));
	}
	else
	{
		lerpValue = 0;
		
	}
	Debug.Log("Moving");
}

void Unit::attackCommand()
{
	int enemyHealth = UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getHealthPoints();
	int damage = this->attackPoints - UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getDefencePoints();
	int newEnemyHealth = enemyHealth - damage;
	UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->setHealthPoints(newEnemyHealth);
	Debug.Log("Attacking.");
}

void Unit::RecieveOrder(RaycastHit Values)
{
	//UnitOrders.clear();
	pathNodes.clear();

	//Target is a unit
	if (Values.transform->gameObject->getComponent<Unit>()!=nullptr)
	{
		Order tempOrder;
		if(Values.transform->gameObject->tag!=gameObject->tag){
			//enemy
			switch (type)
			{
			case Type::Hero:				
				tempOrder.command = Attack;
				//tempOrder.command = Move;

				tempOrder.point = Values.point;
				tempOrder.transform = Values.transform;
				UnitOrders.push_back(tempOrder);
				actionTime = 2;
				break;

			case Type::Soldier:
				tempOrder.command = Attack;
				//tempOrder.command = Move;

				tempOrder.point = Values.point;
				tempOrder.transform = Values.transform;
				UnitOrders.push_back(tempOrder);
				break;
			default:
				//walk to
				break;
			}
		}
		else if(Values.transform->gameObject->tag==0){
			//neutral
			//walk to
			//tempOrder.command = Move;
			//tempOrder.transform = Values.transform;
			//tempOrder.transform = Values.transform;
			//UnitOrders.push_back(tempOrder);
			
		}
		else {
			//friendly
			//walk to
			
		}

	}
	else
	{
		//clicked on terrain
		Order tempOrder;

		switch (type)
		{

		default:
			tempOrder.command = Move;

			tempOrder.point = Values.point;
			tempOrder.transform = Values.transform;
			UnitOrders.push_back(tempOrder);
			break;
		}
	}
}

void Unit::update()
{
	if (UnitOrders.size() > 0) {

		Unit* enemyUnit = UnitOrders.at(0).transform->gameObject->getComponent<Unit>();

		switch (UnitOrders.at(0).command)
		{
		case Command::Move: //MOVE
			MoveCommand();
			break;

		case Command::Attack: //ATTACK
			DirectX::XMVECTOR enemyPos = UnitOrders.at(0).transform->getPosition();
			DirectX::XMVECTOR unitPos = gameObject->transform.getPosition();

			DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(unitPos, enemyPos);
			distance = DirectX::XMVectorGetW(DirectX::XMVector3Length(diff));


			if (enemyUnit != nullptr && enemyUnit->getHealthPoints() >= 0)
			{
				if (distance <= this->attackDistance)
				{
					actionTime += Time.getDeltaTime();
					//Damage enemy
					if (actionTime > 1)
					{
						attackCommand();
						Debug.Log("Enemy Hit!");
						actionTime = 0;
					}
					//UnitOrders.erase(UnitOrders.begin());
				}
				else
				{
					//followCommand()
					//UnitOrders.erase(UnitOrders.begin());
					MoveCommand();
				}
			}
			else
			{
				UnitOrders[0].transform->gameObject->Destroy();
				UnitOrders.erase(UnitOrders.begin());
			}
			break;

		case Command::Gather: //GATHER
			break;

		case Command::Build: //BUILD
			break;

		case Command::Follow: //FOLLOW
			break;

		case Command::Summon: //SUMMON
			break;

		default:
			break;
		}
	}

}
