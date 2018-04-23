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
		this->UnitStats.HealthPoints = 100;
		this->UnitStats.AttackPoints = 13;
		this->UnitStats.DefencePoints = 13;
		this->Resources = 10;
		break;

	case Type::Soldier: //SOLDIER
		this->UnitStats.HealthPoints = 20;
		this->UnitStats.AttackPoints = 4;
		this->UnitStats.DefencePoints = 8;
		this->Resources = 0;
		break;

		case Type::Worker: //WORKER
		this->UnitStats.HealthPoints = 15;
		this->UnitStats.AttackPoints = 0;
		this->UnitStats.DefencePoints = 5;
		this->Resources = 0;
		break;

	case 3: //BUILDING
		this->UnitStats.HealthPoints = 500;
		this->UnitStats.AttackPoints = 0;
		this->UnitStats.DefencePoints = 20;
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
			case Unit::Hero:
				break;
			case Unit::Soldier:
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
		//MoveCommand();

	case Command::Attack: //ATTACK


	case Command::Gather: //GATHER


	case Command::Build: //BUILD


	case Command::Follow: //FOLLOW


	case Command::Summon: //SUMMON


	default:
		break;
	}

}
