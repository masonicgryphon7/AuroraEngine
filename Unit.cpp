#include "Unit.h"
#include "GameObject.h"
#include "GUI_Viewport.h"
#include "InputHandler.h"
#include "GUI.h"
#include "Debug.h"
#include <DirectXMath.h>

Unit::Unit() :Component(-1, "Unit")
{
	actionTime = 10;

	switch (type)
	{
	case Type::Hero: //HERO
		this->healthPoints = 100;
		this->attackPoints = 13;
		this->defencePoints = 13;
		this->attackDistance = 2;
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

Unit::Unit(Type UnitTypeSet) :Component(-1, "Unit")
{
	actionTime = 2;

	switch (UnitTypeSet)
	{
	case Type::Hero: //HERO
		this->healthPoints = 100;
		this->attackPoints = 20;
		this->defencePoints = 9;
		this->attackDistance = 2;
		this->Resources = 0;
		this->type = Hero;
		break;

	case Type::Soldier: //SOLDIER
		this->healthPoints = 20;
		this->attackPoints = 4;
		this->defencePoints = 8;
		this->attackDistance = 2;
		this->Resources = 0;
		this->type = Soldier;
		break;

	case Type::Worker: //WORKER
		this->healthPoints = 15;
		this->attackPoints = 1;
		this->defencePoints = 5;
		this->attackDistance = 2;
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

	case Type::Bank: //Bank

		this->healthPoints = 500;
		this->attackPoints = 0;
		this->defencePoints = 0;
		this->attackDistance = 0;
		this->Resources = 1000;
		this->type = Bank;
		break;
		
	case Type::GoldMine: //NATURE TREES, MINES, ETC

		this->healthPoints = 10000;
		this->attackPoints = 0;
		this->defencePoints = 0;
		this->attackDistance = 0;
		this->Resources = 1000;
		this->type = GoldMine;
		break;

	}
}

Unit::~Unit()
{

}

void Unit::MoveCommand(DirectX::XMVECTOR *goalPos)
{
	DirectX::XMFLOAT3 current;
	DirectX::XMStoreFloat3(&current, gameObject->transform.getPosition());

	DirectX::XMFLOAT3 pointPosition;
	if (goalPos == nullptr)
	{
		DirectX::XMStoreFloat3(&pointPosition, UnitOrders.at(0).point);
	}
	else
	{
	DirectX::XMStoreFloat3(&pointPosition, *goalPos);
	}

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
	//Debug.Log("Moving");
}

void Unit::SecondMoveCommand(DirectX::XMVECTOR * goalPos)
{
	DirectX::XMFLOAT3 current;
	DirectX::XMStoreFloat3(&current, gameObject->transform.getPosition());

	DirectX::XMFLOAT3 pointPosition;
	if (goalPos == nullptr)
	{
		DirectX::XMStoreFloat3(&pointPosition, UnitOrders.at(0).point);
	}
	else
	{
		DirectX::XMStoreFloat3(&pointPosition, *goalPos);
	}

	if (pathNodes.size() == 0)
	{
		lerpValue = 0;
		pathNodes = PathCreator.getPath(current, pointPosition); // Point position
		if (pathNodes.at(pathNodes.size()-1).pathable == PATHABLE_CHECK)
			pathNodes.erase(pathNodes.begin()+pathNodes.size()-1);
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
			//UnitOrders.erase(UnitOrders.begin() + 1);


		}
		gameObject->transform.setPosition(DirectX::XMVectorLerp(gameObject->transform.getPosition(), goal, lerpValue));
	}
	else
	{
		lerpValue = 0;

	}
	//Debug.Log("Moving");
}

void Unit::attackCommand(Unit* targetedUnit)
{
	targetPos = UnitOrders.at(0).transform->getPosition();
	unitPos = gameObject->transform.getPosition();

	if (targetedUnit != nullptr && targetedUnit->getHealthPoints() >= 0)
	{

		if (getDistanceBetweenUnits(unitPos, targetPos) < this->attackDistance)
		{
			actionTime += Time.getDeltaTime();
			//Damage enemy
			if (actionTime > 1)
			{
				//attackEnemy();
				targetedUnit->takeDamage(this->getAttackPoints());
				Debug.Log("Enemy Hit!");
				actionTime = 0;
			}
		}
		else
		{
			//DirectX::XMVECTOR pathOffset = calculateOffsetInPath(unitPos, targetPos);
			//Order tempOrder;
			//tempOrder.command = Command::Move;
			//tempOrder.point = targetedUnit->gameObject->transform.getPosition();//DirectX::XMVectorAdd(targetedUnit->gameObject->transform.getPosition(), DirectX::XMVectorSet(0.5, 0.0, 0.5, 0.0));
			//UnitOrders.insert(UnitOrders.begin(), tempOrder);
			SecondMoveCommand(&targetedUnit->gameObject->transform.getPosition());
		}
	}
	else
	{
		//targetedUnit.destroyUnit();
		UnitOrders.erase(UnitOrders.begin());
	}
}

void Unit::attackEnemy() 
{
	int enemyHealth = UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getHealthPoints();
	int damage = this->attackPoints - UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getDefencePoints();
	int newEnemyHealth = enemyHealth - damage;
	UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->setHealthPoints(newEnemyHealth);
	Debug.Log("Attacking.");
}

void Unit::takeDamage(int attackPoints)
{
	this->setHealthPoints(this->getHealthPoints() - (attackPoints - this->getDefencePoints()));
}

void Unit::FollowCommand()
{
	DirectX::XMFLOAT3 current;
	DirectX::XMStoreFloat3(&current, gameObject->transform.getPosition());

	DirectX::XMFLOAT3 TransformPosition;
	DirectX::XMStoreFloat3(&TransformPosition, UnitOrders.at(0).transform->getPosition());
	int xyz[3] = { (int)TransformPosition.x,(int)TransformPosition.y, (int)TransformPosition.z };


	if (pathNodes.size() == 0)
	{
		pathNodes = PathCreator.getPath(TransformPosition, current);
		lerpValue = 0;
	}
	else
	{
		if (pathNodes.back().position.x != xyz[0] || pathNodes.back().position.y != xyz[1] || pathNodes.back().position.z != xyz[2]) {
			pathNodes = PathCreator.getPath(TransformPosition, current);

		}
	}


	if (pathNodes.size() > 0) {
		lerpValue += Time.getDeltaTime() * 2;
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


		}

		//is final path the same as transform?
		gameObject->transform.setPosition(DirectX::XMVectorLerp(gameObject->transform.getPosition(), goal, lerpValue));
	}
	else
	{
		lerpValue = 0;

	}
	//Debug.Log("Following...");
}

void Unit::gatherCommand(Unit* targetedUnit)
{
	unitPos = gameObject->transform.getPosition();

	if (this->Resources < 100 && e == 0) // worker is not full
	{
		if (getDistanceBetweenUnits(unitPos, targetedUnit->gameObject->transform.getPosition()) < this->attackDistance)
		{
			gatherResources();
		}
		else
		{
			//Order tempOrder;
			//tempOrder.command = Command::Move;
			//tempOrder.point = targetedUnit->gameObject->transform.getPosition();
			//UnitOrders.insert(UnitOrders.begin(), tempOrder);
			SecondMoveCommand(&targetedUnit->gameObject->transform.getPosition());
		}
	}
	else if (this->Resources > 0 && e == 1) // worker has gold
	{
		if (getDistanceBetweenUnits(unitPos, this->homePos->getPosition()) < this->attackDistance)
		{
			dropResources();
		}
		else
		{
			//Order tempOrder;
			//tempOrder.command = Command::Move;
			//tempOrder.point = this->homePos->getPosition();
			//UnitOrders.insert(UnitOrders.begin(), tempOrder);
			SecondMoveCommand(&this->homePos->getPosition());
		}
	}
	
}

void Unit::HeroGatherCommand(Unit * targetedUnit)
{
	unitPos = gameObject->transform.getPosition();

	if (getDistanceBetweenUnits(unitPos, targetedUnit->gameObject->transform.getPosition()) < this->attackDistance && targetedUnit->getResources() > 0)
	{
		actionTime += Time.getDeltaTime();
		if (targetedUnit->type == Bank && actionTime > 4)
		{
			int resourcesLeft = targetedUnit->getResources();
			targetedUnit->setResources(0);
			this->setResources(this->getResources() + resourcesLeft);
			Debug.Log("Resources: ", targetedUnit->getResources());
		}

		if (targetedUnit->type == GoldMine && actionTime > 1)
		{
			gatherResources();
			actionTime = 0;
			Debug.Log("Resources: ", this->getResources());
		}
	}
	else
	{
		//Order tempOrder;
		//tempOrder.command = Command::Move;
		//tempOrder.point = targetedUnit->gameObject->transform.getPosition();
		//UnitOrders.insert(UnitOrders.begin(), tempOrder);
		SecondMoveCommand(&targetedUnit->gameObject->transform.getPosition());
	}
}

void Unit::gatherResources()
{
	actionTime += Time.getDeltaTime();
	if (actionTime > 1)
	{
		int resourcesLeft = UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getResources();
		UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->setResources(resourcesLeft - 20);
		int workersAmount = this->getResources();
		this->setResources(workersAmount + 20);
		Debug.Log("Resource Gathered! Left: ", UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getResources());
		if (this->Resources == 100)
			e = 1;
		actionTime = 0;
	}
}

void Unit::dropCommand(Unit* targetedUnit)
{
	unitPos = gameObject->transform.getPosition();

	if (this->Resources > 0)
	{
		if (getDistanceBetweenUnits(unitPos, this->homePos->getPosition()) < this->attackDistance)
		{
			dropResources();
		}
		else
		{
			Order tempOrder;
			tempOrder.command = Command::Move;
			tempOrder.point = this->homePos->getPosition();
			UnitOrders.insert(UnitOrders.begin(), tempOrder);
		}
	}
}

void Unit::dropResources()
{
	actionTime += Time.getDeltaTime();
	if (actionTime > 1)
	{
		int resourcesInUnit = this->getResources();
		this->setResources(resourcesInUnit - 20);
		Debug.Log("Resources dropped! In worker: ", this->getResources());
		int resourcesInTarget = this->homePos->gameObject->getComponent<Unit>()->getResources();
		this->homePos->gameObject->getComponent<Unit>()->setResources(resourcesInTarget + 20);
		Debug.Log(this->homePos->gameObject->getComponent<Unit>()->getResources());
		if (this->Resources == 0)
			e = 0;
		actionTime = 0;
	}
}

void Unit::destroyUnit()
{
	//UnitOrders[0].transform->gameObject->Destroy();
	gameObject->Destroy();
	
	

}

void Unit::summonWorkerCommand()
{
	GameObject* worker = gScene.createEmptyGameObject(gameObject->transform.getPosition());//gScene.createEmptyGameObject(DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(1.0, 0.0, -3.0, 0.0)));
	worker->name = "Worker"; //+ playerScript->friendlyUnits.size();
	worker->tag = gameObject->tag;
	MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh(4));
	worker->addComponent(meshFilter);
	worker->addComponent(new MaterialFilter(AssetManager.getMaterial(0)));
	Unit *unitWorker = new Unit(Worker);
	unitWorker->setHomePos(&playerScript->friendlyBuildings.at(0)->gameObject->transform);
	worker->addComponent(unitWorker);
	playerScript->friendlyUnits.push_back(unitWorker);
	unitWorker->setPlayerScript(playerScript);
	
	UnitOrders.erase(UnitOrders.begin());
	Order tempOrder;
	tempOrder.command = Move;
	tempOrder.point = DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(1.0, 0.0, -3.0, 0.0));//DirectX::XMVectorSet(1.0, 0.0, 3.0, 0.0);
	unitWorker->getUnitOrdersPointer()->push_back(tempOrder);
}

void Unit::summonSoldierCommand()
{
	GameObject* soldier = gScene.createEmptyGameObject(DirectX::XMVectorSet(2.0, 0.0, 2.0, 0.0));//playerScript->friendlyBuildings.at(0)->gameObject->transform.getPosition());
	soldier->name = "Soldier";// +playerScript->friendlyUnits.size();
	soldier->tag = gameObject->tag;
	MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh(4));
	soldier->addComponent(meshFilter);
	soldier->addComponent(new MaterialFilter(AssetManager.getMaterial(0)));
	Unit *unitSoldier = new Unit(Soldier);
	unitSoldier->setHomePos(&playerScript->friendlyBuildings.at(0)->gameObject->transform);
	soldier->addComponent(unitSoldier);
	playerScript->friendlyUnits.push_back(unitSoldier);
	unitSoldier->setPlayerScript(playerScript);

	UnitOrders.erase(UnitOrders.begin());

	//unitSoldier->RecieveOrder()
}

float Unit::getDistanceBetweenUnits(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos)
{
	DirectX::XMVECTOR diff = DirectX::XMVectorSubtract( targetPos, unitPos);
	float tempDistance = DirectX::XMVectorGetW(DirectX::XMVector3Length(diff));
	return tempDistance;
}

DirectX::XMVECTOR Unit::calculateOffsetInPath(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos)
{
	DirectX::XMVECTOR vectorBetween = DirectX::XMVectorSubtract(unitPos, targetPos);
	DirectX::XMVECTOR normalizedDistance = DirectX::XMVector3Normalize(vectorBetween);
	return normalizedDistance;
}

void Unit::RecieveOrder(RaycastHit Values, int unitTag)
{
	
	UnitOrders.clear();
	pathNodes.clear();

	//Target is a unit
	if (Values.transform->gameObject->getComponent<Unit>()!=nullptr)
	{
		Order tempOrder;
		if (Values.transform->gameObject->tag == unitTag) {
			//friendly
			//walk to

			switch (type)
			{
			case Type::Hero:
				if (Values.transform->gameObject->getComponent<Unit>()->getType() == Worker || Values.transform->gameObject->getComponent<Unit>()->getType() == Soldier)
				{
					tempOrder.command = Follow;
					tempOrder.point = Values.point;
					tempOrder.transform = Values.transform;
					UnitOrders.push_back(tempOrder);
					actionTime = 2;
				}
				if (Values.transform->gameObject->getComponent<Unit>()->getType() == Bank)
				{
					tempOrder.command = HeroGather;
					tempOrder.point = Values.point;
					tempOrder.transform = Values.transform;
					UnitOrders.push_back(tempOrder);
					actionTime = 2;
				}
				break;

			case Type::Soldier:
				tempOrder.command = Follow;
				tempOrder.point = Values.point;
				tempOrder.transform = Values.transform;
				UnitOrders.push_back(tempOrder);
				break;

			case Type::Worker:
				if (Values.transform->gameObject->getComponent<Unit>()->getType()== Hero || Values.transform->gameObject->getComponent<Unit>()->getType() == Soldier)
				{
					tempOrder.command = Follow;
					tempOrder.point = Values.point;
					tempOrder.transform = Values.transform;
					UnitOrders.push_back(tempOrder);
				}
				if (Values.transform->gameObject->getComponent<Unit>()->getType() == Bank)
				{
					tempOrder.command = Drop;
					tempOrder.point = Values.point;
					tempOrder.transform = Values.transform;
					UnitOrders.push_back(tempOrder);
					actionTime = 2;
				}
				break;
			}
		}
		else if(Values.transform->gameObject->tag != unitTag && unitTag != 3 && unitTag != 0){ //!=gameObject->tag){
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
		else if(Values.transform->gameObject->tag==3){
			//Resource
			//Gather

			switch (type)
			{
			case Type::Hero:
				tempOrder.command = HeroGather;
				tempOrder.point = Values.point;
				tempOrder.transform = Values.transform;
				UnitOrders.push_back(tempOrder);
				actionTime = 2;

				break;


			case Type::Worker:
				if (Values.transform->gameObject->getComponent<Unit>()->type == GoldMine)
				{
					tempOrder.command = Gather;
					tempOrder.point = Values.point;
					tempOrder.transform = Values.transform;
					UnitOrders.push_back(tempOrder);
					actionTime = 2;
				}

				break;

			default:
				//walk to
				break;
			}
			
		}
		else if (Values.transform->gameObject->tag == 0) {
			//Terrain
			//Walk 
		}
		else {
			
		}

	}
	else
	{
		//clicked on terrain
		Order tempOrder;

		switch (type)
		{
		case Type::Bank:
			break;

		default:
			tempOrder.command = Move;

			tempOrder.point = Values.point;
			tempOrder.transform = Values.transform;
			UnitOrders.push_back(tempOrder);
			break;
		}
	}
}

void Unit::RecieveOrder(OPTIONS option)
{
	UnitOrders.clear();
	pathNodes.clear();

	Order tempOrder;
	switch (type)
	{
	case Type::Bank:
		if (option == Option0)
		{
			tempOrder.command = SummonWorker;
			UnitOrders.push_back(tempOrder);
		}
		else if (option == Option1)
		{
			tempOrder.command = SummonSoldier;
			UnitOrders.push_back(tempOrder);
		}
		else if (option == Option2)
		{

		}
		else if (option == Option3)
		{

		}
		break;

	case Type::Hero:
		if (option == Option0)
		{

		}
		else if (option == Option1)
		{

		}
		else if (option == Option2)
		{

		}
		else if (option == Option3)
		{

		}
		break;
	}
}

void Unit::update()
{
	if (UnitOrders.size() > 0) {


		switch (UnitOrders.at(0).command)
		{
		case Command::Move: //MOVE
			MoveCommand(&UnitOrders.at(0).point);
			break;

		case Command::Attack: //ATTACK
		{
			Unit* targetedUnit = UnitOrders.at(0).transform->gameObject->getComponent<Unit>();
			attackCommand(targetedUnit);			
		}
			break;

		case Command::Gather: //GATHER
		{
			Unit* targetedUnit = UnitOrders.at(0).transform->gameObject->getComponent<Unit>();
			gatherCommand(targetedUnit);
		}
			break;

		case Command::HeroGather: //HEROGATHER
		{
			Unit * targetedUnit = UnitOrders.at(0).transform->gameObject->getComponent<Unit>();
			HeroGatherCommand(targetedUnit);
		}
			break;

		case Command::Drop:
		{
			Unit * targetedUnit = UnitOrders.at(0).transform->gameObject->getComponent<Unit>();
			dropCommand(targetedUnit);
		}
			break;

		case Command::Build: //BUILD

			break;

		case Command::Follow: //FOLLOW
			FollowCommand();
			break;

		case Command::SummonWorker: //SUMMON
			summonWorkerCommand();
			break;

		case Command::SummonSoldier:
			summonSoldierCommand();
			break;

		default:
			break;
		}
	}

	//if (Input.GetKeyUp(KeyCode::Alpha1)) {
	//	summonWorkerCommand();
	//	}

	//if (Input.GetKeyUp(KeyCode::Alpha2)) {
	//	summonSoldierCommand();
	//}
}
