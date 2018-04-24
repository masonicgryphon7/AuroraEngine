#include "Unit.h"
#include "GameObject.h"
#include "GUI_Viewport.h"
#include "InputHandler.h"
#include "GUI.h"

Unit::Unit()
{
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

Unit::~Unit()
{
}

void Unit::MoveCommand()
{
	//DirectX::XMFLOAT3 current;
	//DirectX::XMStoreFloat3(&current, gameObject->transform.getPosition());

	//if (Input.GetKeyDown(KeyCode::LeftMouse))
	//{
	//	RaycastHit hit;

	//	Ray ray = editorCamera->calculateScreenPointToRay(DirectX::XMVectorSet(Input.GetMousePosition().x, Input.GetMousePosition().y, 0, 0));
	//	gPhysics.Raycast(ray, hit);

	//	//hit.transform->setPosition(DirectX::XMVectorSet(0, 10, 0, 0));
	//	if (hit.transform != nullptr) {
	//		gameObject->name = "Hit obj";
	//		DirectX::XMVECTOR hitPos = DirectX::XMVectorAdd(editorCamera->gameObject->transform.getPosition(), DirectX::XMVectorScale(ray.direction, hit.distance));
	//		Debug.Log("moving to", DirectX::XMVectorGetX(hitPos), ",", DirectX::XMVectorGetY(hitPos), ",", DirectX::XMVectorGetZ(hitPos));
	//		DirectX::XMStoreFloat3(&goalPos, hitPos);
	//		lerpValue = 0;

	//		pathNodes = PathCreator.getPath(current, goalPos);
	//	}
	//	else
	//	{
	//		gameObject->name = "Missed obj";
	//		//Console.warning("ray" + Input.GetMouseCoordinates().x+ Input.GetMouseCoordinates().y);
	//	}
	//}

	//if (pathNodes.size() > 0) {
	//	lerpValue += Time.getDeltaTime() * 10;
	//	if (lerpValue > 1) {
	//		lerpValue = 1;
	//	}
	//	DirectX::XMVECTOR goal = DirectX::XMVectorSet(pathNodes.at(0).position.x, pathNodes.at(0).position.y, pathNodes.at(0).position.z, 0);
	//	DirectX::XMFLOAT3 goalVec;
	//	DirectX::XMStoreFloat3(&goalVec, goal);


	//	if (DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract(goal, gameObject->transform.getPosition())))<EPSILON &&pathNodes.size() > 1) {
	//		pathNodes.erase(pathNodes.begin());
	//		//current = goalVec;
	//		goal = DirectX::XMVectorSet(pathNodes.at(0).position.x, pathNodes.at(0).position.y, pathNodes.at(0).position.z, 0);
	//		lerpValue = 0;

	//	}
	//	gameObject->transform.setPosition(DirectX::XMVectorLerp(gameObject->transform.getPosition(), goal, lerpValue));
	//}
	//else
	//{
	//	lerpValue = 0;
	//}
}

void Unit::RecieveOrder(RaycastHit Values)
{
	//Target is a unit
	if (Values.transform->gameObject->getComponent<Unit>()!=nullptr)
	{
		if(Values.transform->gameObject->tag!=gameObject->tag){
			//enemy
			switch (type)
			{
			case Type::Hero:
				break;
			case Type::Soldier:
				break;
			default:
				//walk to
				break;
			}
		}
		else if(Values.transform->gameObject->tag==0){
			//neutral
			//walk to
			
		}
		else {
			//friendly
			//walk to
			
		}

	}
	else
	{
		//clicked on terrain
	}
}

void Unit::update()
{
	switch (UnitOrders.at(0).command)
	{
	case Command::Move: //MOVE
		MoveCommand();

	case Command::Attack: //ATTACK
		DirectX::XMVECTOR enemyPos = UnitOrders.at(0).transform->getPosition();
		DirectX::XMVECTOR unitPos = gameObject->transform.getPosition();

		DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(unitPos, enemyPos);
		float distance = DirectX::XMVectorGetW(DirectX::XMVector3Length(diff));

		if (distance < this->attackDistance)
		{
			// Damage enemy
			int enemyHealth = UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getHealthPoints();
			int damage = this->attackPoints - UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getDefencePoints();
			int newEnemyHealth = enemyHealth - damage;
			UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->setHealthPoints(newEnemyHealth);
		}
		else
		{
			//followCommand()
		}

	case Command::Gather: //GATHER


	case Command::Build: //BUILD


	case Command::Follow: //FOLLOW


	case Command::Summon: //SUMMON


	default:
		break;
	}

}
