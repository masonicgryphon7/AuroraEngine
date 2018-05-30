#include "Unit.h"
#include "GameObject.h"
#include "GUI_Viewport.h"
#include "InputHandler.h"
#include "GUI.h"
#include "Debug.h"
#include <DirectXMath.h>
#include <math.h>
#include "Projectile.h"
#include "Console.h"
Unit::Unit() :Component(-1, "Unit")
{
	actionTime = 10;

	switch (type)
	{
	case Type::Hero: //HERO
		this->healthPoints = this->maxHealthPoints = 100;
		this->attackPoints = 15;
		this->defencePoints = 5;
		this->attackDistance = 10;
		this->Resources = 10;
		break;

	case Type::Soldier: //SOLDIER
		this->healthPoints = this->maxHealthPoints = 20;
		this->attackPoints = 4;
		this->defencePoints = 8;
		this->attackDistance = 6;
		this->Resources = 0;
		break;

	case Type::Worker: //WORKER
		this->healthPoints = this->maxHealthPoints = 15;
		this->attackPoints = 1;
		this->defencePoints = 5;
		this->attackDistance = 1;
		this->Resources = 0;
		break;

	case 3: //BUILDING
		this->healthPoints = this->maxHealthPoints = 500;
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
	srand(time(NULL));
	float x = (rand() % 100) / 200.0f;
	float z = (rand() % 100) / 200.0f;
	offset = DirectX::XMVectorSet(x, 0.0f, z, 0.0f);

}

Unit::Unit(Type UnitTypeSet) :Component(-1, "Unit")
{
	actionTime = 2;
	dieTime = 0;

	switch (UnitTypeSet)
	{
	case Type::Hero: //HERO
		this->healthPoints = this->maxHealthPoints = 100;
		this->attackPoints = 16;
		this->defencePoints = 10; // 10
		this->attackDistance = 10;
		this->Resources = 0;
		this->type = Hero;
		break;

	case Type::Soldier: //SOLDIER
		this->healthPoints = this->maxHealthPoints = 30;
		this->attackPoints = 13;
		this->defencePoints = 7;
		this->attackDistance = 6;
		this->Resources = 0;
		this->type = Soldier;
		break;

	case Type::Worker: //WORKER
		this->healthPoints = this->maxHealthPoints = 15;
		this->attackPoints = 11;
		this->defencePoints = 5;
		this->attackDistance = 3;
		this->Resources = 0;
		this->type = Worker;
		break;

	case Type::Barrack: //BUILDING
		this->healthPoints = this->maxHealthPoints = 20;
		this->attackPoints = 0;
		this->defencePoints = 20;
		this->attackDistance = 0;
		this->Resources = 0;
		this->type = Barrack;
		break;

	case Type::Bank: //Bank

		this->healthPoints = this->maxHealthPoints = 20;
		this->attackPoints = 0;
		this->defencePoints = 0;
		this->attackDistance = 0;
		this->Resources = 1000;
		this->type = Bank;
		break;

	case Type::GoldMine: //NATURE TREES, MINES, ETC

		this->healthPoints = this->maxHealthPoints = 20;
		this->attackPoints = 0;
		this->defencePoints = 0;
		this->attackDistance = 0;
		this->Resources = 100000;
		this->type = GoldMine;
		break;

	}
	srand(time(NULL));
	float x = (rand() % 100) / 200.0f;
	float z = (rand() % 100) / 200.0f;
	offset = DirectX::XMVectorSet(x, 0.0f, z, 0.0f);

}

Unit::~Unit()
{

}

Command Unit::getUnitCommand()
{
	if (this->UnitOrders.size() > 0)
		return this->UnitOrders[0].command;
	else
	{ 
		return Idle; 
		this->soundAction = 0;
	};
}

void Unit::setUnitOrder(Order newOrder)
{
	if (this->UnitOrders.empty() || (!this->UnitOrders.empty() && this->UnitOrders.at(0).command != Die))
	{
		UnitOrders.clear();
		this->UnitOrders.push_back(newOrder);
	}
}

void Unit::MoveCommand(DirectX::XMVECTOR *goalPos)
{
	if (gameObject->unitIsAvtive == true)
	{
		this->soundAction = 1;

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
			previousPos = gameObject->transform.getPosition();

			start = std::clock();
			float time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
			Console.success("This is a time measurement", time);
			lerpValue += Time.getDeltaTime() * 10;
			if (lerpValue > 1) {
				lerpValue = 0;
			}
		}

		if (pathNodes.size() > 0) {
			lerpValue += Time.getDeltaTime() * 10;
			if (lerpValue > 1) {
				lerpValue = 1;
			}
			DirectX::XMVECTOR goal =DirectX::XMVectorAdd(offset, DirectX::XMVectorSet(pathNodes.at(0).position.x, pathNodes.at(0).position.y, pathNodes.at(0).position.z, 0));
			DirectX::XMFLOAT3 goalVec;
			DirectX::XMStoreFloat3(&goalVec, goal);

			DirectX::XMVECTOR forward = gameObject->transform.getForward();
			DirectX::XMVECTOR currentPoint = gameObject->transform.getPosition();
			DirectX::XMVECTOR goalPoint = DirectX::XMVectorSet(goalVec.x, goalVec.y, goalVec.z, 0.0);
			DirectX::XMVECTOR currentToGoal = DirectX::XMVectorSubtract(goalPoint, currentPoint);


			//	gameObject->transform.setForward(gameObject->transform.getForward());
			if (DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract(goal, gameObject->transform.getPosition()))) < EPSILON &&pathNodes.size() > 1) {
				previousPos =goal;
				pathNodes.erase(pathNodes.begin());
			
				goal = DirectX::XMVectorSet(pathNodes.at(0).position.x, pathNodes.at(0).position.y, pathNodes.at(0).position.z, 0);
			
				lerpValue = 0;

			}
			else if (DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract(goal, gameObject->transform.getPosition()))) < EPSILON &&pathNodes.size() == 1) {
				previousPos = goal;
				pathNodes.erase(pathNodes.begin());
				
				lerpValue = 0;
				UnitOrders.erase(UnitOrders.begin());
				this->soundAction = 0;
			}
			gameObject->transform.setPosition(DirectX::XMVectorLerp(previousPos, goal, lerpValue));

			DirectX::XMVECTOR directionVector = DirectX::XMVector3Normalize( DirectX::XMVectorSubtract(goalPoint, previousPos));
			float zComponent = DirectX::XMVectorGetZ(directionVector);
			float xComponent = DirectX::XMVectorGetX(directionVector);

			if (zComponent< 0 && xComponent==0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0,90,0,0));

			}else if (zComponent > 0 && xComponent==0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 270, 0, 0));

			}else if (xComponent> 0 && zComponent==0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 0, 0, 0));

			}else if (xComponent < 0 && zComponent==0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 180, 0, 0));

			}
			else if (xComponent < 0 && zComponent < 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 135, 0, 0));

			}
			else if (xComponent < 0 && zComponent > 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 225, 0, 0));

			}
			else if (xComponent > 0 && zComponent > 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 315, 0, 0));

			}
			else if (xComponent > 0 && zComponent < 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 45, 0, 0));

			}


			
			
		}

		else
		{
			lerpValue = 0;
		}
	}
}

void Unit::SecondMoveCommand(DirectX::XMVECTOR * goalPos)
{
	if (gameObject->unitIsAvtive == true)
	{
		this->soundAction = 1;
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
			if (this->getType() == Worker && this->gatherCollect == true)
				worker_has_path = true;
			else
				worker_has_path = false;
			pathNodes = PathCreator.getPath(current, pointPosition); // Point position
			previousPos = gameObject->transform.getPosition();

			if (pathNodes.at(pathNodes.size() - 1).pathable == PATHABLE_CHECK)
				pathNodes.erase(pathNodes.begin() + pathNodes.size() - 1);
		}

		if (pathNodes.size() > 0) {
			lerpValue += Time.getDeltaTime() * 10;
			if (lerpValue > 1) {
				lerpValue = 1;
			}
	
			DirectX::XMVECTOR goal = DirectX::XMVectorSet(pathNodes.at(0).position.x, pathNodes.at(0).position.y, pathNodes.at(0).position.z, 0);
			DirectX::XMFLOAT3 goalVec;
			DirectX::XMStoreFloat3(&goalVec, goal);


			if (DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract(goal, gameObject->transform.getPosition()))) < EPSILON &&pathNodes.size() > 1) {
				previousPos = goal;
				if(this->getType() == Worker && worker_has_path == true)
				worker_return_path.push_back(pathNodes[0]);
				
				pathNodes.erase(pathNodes.begin());

				goal = DirectX::XMVectorSet(pathNodes.at(0).position.x, pathNodes.at(0).position.y, pathNodes.at(0).position.z, 0);
				lerpValue = 0;
			}
			else if (DirectX::XMVectorGetW(DirectX::XMVector3Length(DirectX::XMVectorSubtract(goal, gameObject->transform.getPosition()))) < EPSILON &&pathNodes.size() == 1) {
				previousPos = goal;
				if (this->getType() == Worker && worker_has_path == true)
				worker_return_path.push_back(pathNodes[0]);
				pathNodes.erase(pathNodes.begin());
				lerpValue = 0;
				//UnitOrders.erase(UnitOrders.begin() + 1);
				this->soundAction = 0;

			}
			if (pathNodes.size() == 0 && this->getType() == Worker && worker_has_path && this->getUnitCommand() == Gather)
			{
				std::reverse(worker_return_path.begin(), worker_return_path.end());
				pathNodes = worker_return_path;
				worker_return_path.clear();
			}
			gameObject->transform.setPosition(DirectX::XMVectorLerp(previousPos, goal, lerpValue));

			DirectX::XMVECTOR directionVector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(goal, previousPos));
			float zComponent = DirectX::XMVectorGetZ(directionVector);
			float xComponent = DirectX::XMVectorGetX(directionVector);

			if (zComponent< 0 && xComponent == 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));

			}
			else if (zComponent > 0 && xComponent == 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 270, 0, 0));

			}
			else if (xComponent> 0 && zComponent == 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 0, 0, 0));

			}
			else if (xComponent < 0 && zComponent == 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 180, 0, 0));

			}
			else if (xComponent < 0 && zComponent < 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 135, 0, 0));

			}
			else if (xComponent < 0 && zComponent > 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 225, 0, 0));

			}
			else if (xComponent > 0 && zComponent > 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 315, 0, 0));

			}
			else if (xComponent > 0 && zComponent < 0) {
				gameObject->transform.setRotation(DirectX::XMVectorSet(0, 45, 0, 0));

			}
		}
		else
		{
			lerpValue = 0;
		}
	}
	//Debug.Log("Moving");
}

void Unit::attackCommand(Unit* targetedUnit)
{
	targetPos = UnitOrders.at(0).transform->getPosition();
	unitPos = gameObject->transform.getPosition();

	if (targetedUnit->gameObject->unitIsAvtive == true)
	{
		if (targetedUnit != nullptr && targetedUnit->getHealthPoints() > 0)
		{

			if (getDistanceBetweenUnits(unitPos, targetPos) < this->attackDistance)
			{
				actionTime += Time.getDeltaTime();
				//Damage enemy
				if (actionTime > 1)
				{
					//attackEnemy();
					targetedUnit->takeDamage(this->getAttackPoints(), gameObject->getComponent<Unit>());
					//Debug.Log("Enemy Hit!");
					actionTime = 0;
					this->soundAction = 2;
					GameObject* projectile = gScene.createEmptyGameObject(unitPos);//playerScript->friendlyBuildings.at(0)->gameObject->transform.getPosition());
					Projectile* projectileComponent = new Projectile(unitPos, targetPos, targetedUnit);
					MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh("Projektil_Mesh"));
					projectile->addComponent(meshFilter);
					projectile->addComponent(projectileComponent);
 					projectile->addComponent(new MaterialFilter(AssetManager.getMaterial("Bullet")));  
					DirectX::XMVECTOR directionVector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetPos, unitPos));
					float zComponent = DirectX::XMVectorGetZ(directionVector);
					float xComponent = DirectX::XMVectorGetX(directionVector);

					if (zComponent< 0 && xComponent == 0) {
						gameObject->transform.setRotation(DirectX::XMVectorSet(0, 90, 0, 0));
						projectileComponent->setRotation(DirectX::XMVectorSet(0, 90, 0, 0));

					}
					else if (zComponent > 0 && xComponent == 0) {
						gameObject->transform.setRotation(DirectX::XMVectorSet(0, 270, 0, 0));
						projectileComponent->setRotation(DirectX::XMVectorSet(0, 270, 0, 0));

					}
					else if (xComponent> 0 && zComponent == 0) {
						gameObject->transform.setRotation(DirectX::XMVectorSet(0, 0, 0, 0));
						projectileComponent->setRotation(DirectX::XMVectorSet(0, 0, 0, 0));

					}
					else if (xComponent < 0 && zComponent == 0) {
						gameObject->transform.setRotation(DirectX::XMVectorSet(0, 180, 0, 0));
						projectileComponent->setRotation(DirectX::XMVectorSet(0, 180, 0, 0));

					}
					else if (xComponent < 0 && zComponent < 0) {
						gameObject->transform.setRotation(DirectX::XMVectorSet(0, 135, 0, 0));
						projectileComponent->setRotation(DirectX::XMVectorSet(0, 135, 0, 0));

					}
					else if (xComponent < 0 && zComponent > 0) {
						gameObject->transform.setRotation(DirectX::XMVectorSet(0, 225, 0, 0));
						projectileComponent->setRotation(DirectX::XMVectorSet(0, 225, 0, 0));

					}
					else if (xComponent > 0 && zComponent > 0) {
						gameObject->transform.setRotation(DirectX::XMVectorSet(0, 315, 0, 0));
						projectileComponent->setRotation(DirectX::XMVectorSet(0, 315, 0, 0));

					}
					else if (xComponent > 0 && zComponent < 0) {
						gameObject->transform.setRotation(DirectX::XMVectorSet(0, 45, 0, 0));
						projectileComponent->setRotation(DirectX::XMVectorSet(0, 45, 0, 0));

					}
				}
			}
			else
			{
				SecondMoveCommand(&targetedUnit->gameObject->transform.getPosition());
			}
		}
		else {
			UnitOrders.erase(UnitOrders.begin());
		}
	}
	else {
		UnitOrders.erase(UnitOrders.begin());
		this->soundAction = 0;
	}

}

void Unit::attackEnemy()
{
	int enemyHealth = UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getHealthPoints();
	int damage = this->attackPoints - UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getDefencePoints();
	int newEnemyHealth = enemyHealth - damage;
	UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->setHealthPoints(newEnemyHealth);
	//Debug.Log("Attacking.");
}

void Unit::takeDamage(int attackPoints, Unit* attackedBy)
{
	if(attackPoints > this->getDefencePoints())
		this->setHealthPoints(this->getHealthPoints() - (attackPoints - this->getDefencePoints()));

	this->soundAction = 3;

	if (this->UnitOrders.size() == 0 && this->getType() == Soldier && this->getType() == Hero && this->getType() == Worker)
	{
		Order temporder;
		temporder.point = attackedBy->gameObject->transform.getPosition();
		temporder.transform = &attackedBy->gameObject->transform;
		temporder.command = Command::Attack;
		UnitOrders.push_back(temporder);
	}

	if (healthPoints <= 0)
	{
		gameObject->raycastOption = RayCastOptions::NONE;
		gameObject->unitIsAvtive = false;
		Order tempOrder;
		tempOrder.command = Die;

		UnitOrders.insert(UnitOrders.begin(), tempOrder);
	}
}

void Unit::takeFireDamage(float attackPoints)
{
	this->soundAction = 4;
	this->setHealthPoints(this->getHealthPoints() - attackPoints);
	if (healthPoints <= 0)
	{

		gameObject->raycastOption = RayCastOptions::NONE;
		gameObject->unitIsAvtive = false;
		Order tempOrder;
		tempOrder.command = Die;

		UnitOrders.insert(UnitOrders.begin(), tempOrder);
	}
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
	if (targetedUnit->gameObject != nullptr) //&& this->homePos != nullptr && this->homePos->gameObject != nullptr)
	{
		unitPos = gameObject->transform.getPosition();
		if (UnitOrders.at(0).transform->gameObject->unitIsAvtive == false || targetedUnit->gameObject->unitIsAvtive == false)
		{
			UnitOrders.erase(UnitOrders.begin());
		}
		else {
			if (this->Resources < 100 && e == 0 && targetedUnit->gameObject->unitIsAvtive == true) // worker is not full
			{
				if (getDistanceBetweenUnits(unitPos, targetedUnit->gameObject->transform.getPosition()) < this->attackDistance)
				{
					gatherResources();
				}
				else
				{
					SecondMoveCommand(&targetedUnit->gameObject->transform.getPosition());
				}
			}
			else if (this->Resources > 0 && e == 1) //&& this->homePos->gameObject != nullptr && this->homePos->gameObject->unitIsAvtive == true) // worker has gold
			{
				float tempDistance = 100000;
				float len = 100000.0;
				Transform* dropPos = &this->gameObject->transform;
				for (int i = 0; i < gamemanager.buildingLists[this->gameObject->tag].size(); i++)
				{
					if (gamemanager.buildingLists[this->gameObject->tag][i]->getType() == Bank)
					{
						len = getDistanceBetweenUnits(unitPos, gamemanager.buildingLists[this->gameObject->tag][i]->gameObject->transform.getPosition());
						if(len < tempDistance)
						{
							dropPos = &gamemanager.buildingLists[this->gameObject->tag][i]->gameObject->transform;
						}

					}
				}
				if (len < this->attackDistance)
				{
					dropResources(dropPos);
				}
				else if(dropPos != &this->gameObject->transform)
				{
					SecondMoveCommand(&dropPos->getPosition());
				}
			}
		}
	}
	else
	{
		UnitOrders.erase(UnitOrders.begin());
	}
}

void Unit::HeroGatherCommand(Unit * targetedUnit)
{
	unitPos = gameObject->transform.getPosition();

	if (getDistanceBetweenUnits(unitPos, targetedUnit->gameObject->transform.getPosition()) < this->attackDistance && targetedUnit->getResources() > 0 && targetedUnit->gameObject->unitIsAvtive == true)
	{
		actionTime += Time.getDeltaTime();
	

		if (targetedUnit->type == GoldMine && actionTime > 2)
		{
			this->goldMineCollecting = targetedUnit->gameObject;

			gatherResources();
			actionTime = 0;
			//Debug.Log("Resources: ", this->getResources());
		}
	}
	else if(targetedUnit->gameObject->unitIsAvtive == true)
	{
		SecondMoveCommand(&targetedUnit->gameObject->transform.getPosition());
	}
	else {
		UnitOrders.erase(UnitOrders.begin());
	}
}

void Unit::gatherResources()
{
	this->soundAction = 5;
	
	if (UnitOrders.at(0).transform->gameObject->unitIsAvtive == true)
	{
		if (actionTime > 1)
		{
			gatherCollect = true;
			this->isCollectingGold = true;
			int resourcesLeft = UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getResources();
			UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->setResources(resourcesLeft - 20);
			int workersAmount = this->getResources();
			this->setResources(workersAmount + 20);
			//Debug.Log("Resource Gathered! Left: ", UnitOrders.at(0).transform->gameObject->getComponent<Unit>()->getResources());
			if (this->Resources == 100)
				e = 1;
			
		}
		else
		{
			gatherCollect = false;
		}
	}
	else {
		UnitOrders.erase(UnitOrders.begin());
	}
}

void Unit::dropCommand(Unit* targetedUnit)
{
	unitPos = gameObject->transform.getPosition();
	findClosest = 10000;//////////////
	
	this->homePos = &targetedUnit->gameObject->transform;

	if (this->homePos->gameObject->unitIsAvtive == true)
	{
		if (this->Resources > 0)
		{
			if (getDistanceBetweenUnits(unitPos, this->homePos->getPosition()) < this->attackDistance)
			{
				dropResources(this->homePos);
			}
			else
			{
				Order tempOrder;
				tempOrder.command = Command::Move;
				tempOrder.point = this->homePos->getPosition();
				UnitOrders.insert(UnitOrders.begin(), tempOrder);
			}
		}
		else
		{
			DirectX::XMVECTOR orderPoint = this->gameObject->transform.getPosition();
			Transform *orderTransform = &this->gameObject->transform;

			for (int i = 0; i < gamemanager.buildingLists[0].size(); i++)
			{
				if (gamemanager.buildingLists[0][i]->type == GoldMine)
				{
					int temp = getDistanceBetweenUnits(unitPos, gamemanager.buildingLists[0][i]->gameObject->transform.getPosition());
					if (temp < findClosest)
					{
						findClosest = temp;
						orderPoint = gamemanager.buildingLists[0][i]->gameObject->transform.getPosition();
						orderTransform = &gamemanager.buildingLists[0][i]->gameObject->transform;
					}
				}
			}

			UnitOrders.erase(UnitOrders.begin());

			Order tempOrder;
			tempOrder.command = Command::Gather;
			tempOrder.transform = orderTransform;
			tempOrder.point = orderPoint;
			UnitOrders.insert(UnitOrders.begin(), tempOrder);
		}
	}
	else {
		UnitOrders.erase(UnitOrders.begin());
	}
}

void Unit::dropResources(Transform* dropPos)
{
	this->soundAction = 6;
	actionTime += Time.getDeltaTime();
	if (dropPos->gameObject->unitIsAvtive == true)
	{
		if (actionTime > 1)
		{
			gatherCollect = true;
			int resourcesInUnit = this->getResources();
			this->setResources(resourcesInUnit - 20);
			//Debug.Log("Resources dropped! In worker: ", this->getResources());
			//int resourcesInTarget = this->homePos->gameObject->getComponent<Unit>()->getResources();
			int resourcesInTarget = gamemanager.unitLists[this->gameObject->tag][0]->getResources();
			//this->homePos->gameObject->getComponent<Unit>()->setResources(resourcesInTarget + 20);
			gamemanager.unitLists[this->gameObject->tag][0]->setResources(resourcesInTarget + 20);
			//Debug.Log(this->homePos->gameObject->getComponent<Unit>()->getResources());
			if (this->Resources == 0)
				e = 0;
			actionTime = 0;
		}
		else
		{
			gatherCollect = false;
		}
	}
	else {
		UnitOrders.erase(UnitOrders.begin());
	}
}

void Unit::destroyUnit()
{
	//UnitOrders[0].transform->gameObject->Destroy();
	gameObject->Destroy();
}

void Unit::summonWorkerCommand()
{
	if (this->gameObject->unitIsAvtive == true)
	{
		int temp = 0;
		Unit* temp2;
		
		temp = gamemanager.unitLists[gameObject->tag][0]->getResources();
		temp2 = gamemanager.unitLists[gameObject->tag][0];

		if (temp > 40)
		{
			GameObject* worker = gScene.createEmptyGameObject(gameObject->transform.getPosition());//playerScript->friendlyBuildings.at(0)->gameObject->transform.getPosition());
			worker->name = "Worker" + std::to_string(gamemanager.unitLists[gameObject->tag].size());
			worker->tag = gameObject->tag;
			MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh("Worker1"));
			worker->addComponent(meshFilter);
			worker->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));
			Unit *unitworker = new Unit(Worker);
			unitworker->setHomePos(&this->gameObject->transform);//&playerScript->friendlyBuildings.at(0)->gameObject->transform);
			worker->addComponent(unitworker );
			//playerScript->friendlyUnits.push_back(unitworker);
			unitworker->setPlayerScript(playerScript);
			gamemanager.unitLists[gameObject->tag].push_back(unitworker);

			Order tempOrder;
			tempOrder.command = Move;
			tempOrder.point = DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(-7.0, 0.0, 0.0, 0.0));//DirectX::XMVectorSet(1.0, 0.0, 3.0, 0.0);
			unitworker->getUnitOrdersPointer()->push_back(tempOrder);

			temp2->setResources(temp - 60);
		}
		UnitOrders.erase(UnitOrders.begin());
	}
}

void Unit::convertToSoldierCommand(Unit* targetedUnit)
{
	if (UnitOrders.at(0).transform->gameObject->unitIsAvtive == true)
	{
		targetPos = UnitOrders.at(0).transform->getPosition();
		unitPos = gameObject->transform.getPosition();

		if (getDistanceBetweenUnits(unitPos, targetPos) < this->attackDistance)
		{
			gameObject->name = "Soldier" + std::to_string(gamemanager.unitLists[gameObject->tag].size());
			gameObject->getComponent<MeshFilter>()->setMesh(AssetManager.getMesh("Soldier1"));
			gameObject->getComponent<MaterialFilter>()->setMaterialFilter(AssetManager.getMaterial("SoldierMaterial"));
			gameObject->getComponent<Unit>()->type = Soldier;
			gameObject->getComponent<Unit>()->healthPoints = 20;
			gameObject->getComponent<Unit>()->attackDistance = 2;
			gameObject->getComponent<Unit>()->attackPoints = 8;
			gameObject->getComponent<Unit>()->defencePoints = 4;
			gameObject->getComponent<Unit>()->Resources = 0;

			UnitOrders.erase(UnitOrders.begin());
		}
		else
		{
			SecondMoveCommand(&targetedUnit->gameObject->transform.getPosition());
		}
	}
}

void Unit::summonSoldierCommand()
{
	if (this->gameObject->unitIsAvtive == true)
	{
		int temp = 0;
		Unit* temp2;

		temp = gamemanager.unitLists[gameObject->tag][0]->getResources();
		temp2 = gamemanager.unitLists[gameObject->tag][0];
		
		if (temp > 40)
		{
			GameObject* soldier = gScene.createEmptyGameObject(gameObject->transform.getPosition());//playerScript->friendlyBuildings.at(0)->gameObject->transform.getPosition());
			soldier->name = "Soldier" + std::to_string(gamemanager.unitLists[gameObject->tag].size());
			soldier->tag = gameObject->tag;
			MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh("Soldier1"));
			soldier->addComponent(meshFilter);
			soldier->addComponent(new MaterialFilter(AssetManager.getMaterial("SoldierMaterial")));
			Unit *unitSoldier = new Unit(Soldier);
			unitSoldier->setHomePos(&gameObject->transform);//&playerScript->friendlyBuildings.at(0)->gameObject->transform);
			soldier->addComponent(unitSoldier);
			//playerScript->friendlyUnits.push_back(unitSoldier);
			unitSoldier->setPlayerScript(playerScript);
			gamemanager.unitLists[gameObject->tag].push_back(unitSoldier);

			UnitOrders.erase(UnitOrders.begin());
			Order tempOrder;
			tempOrder.command = Move;
			tempOrder.point = DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(1.0, 0.0, -8.0, 0.0));//DirectX::XMVectorSet(1.0, 0.0, 3.0, 0.0);
			unitSoldier->getUnitOrdersPointer()->push_back(tempOrder);
			
			temp2->setResources(temp - 60);
		}
	}
}

void Unit::takeBuildingCommand(Unit * targetedUnit)
{
	if (UnitOrders.at(0).transform->gameObject->unitIsAvtive == true)
	{
		targetPos = UnitOrders.at(0).transform->getPosition();
		unitPos = gameObject->transform.getPosition();

		if (getDistanceBetweenUnits(unitPos, targetPos) < this->attackDistance)
		{
			actionTime += Time.getDeltaTime();
			
			if (actionTime > 1)
			{
				//attackEnemy();
				for (int i = 0; i < gamemanager.buildingLists[targetedUnit->gameObject->tag].size(); i++)
				{
					if (gamemanager.buildingLists[targetedUnit->gameObject->tag][i] == targetedUnit)
					{
						this->soundAction = 8;

						gamemanager.buildingLists[targetedUnit->gameObject->tag].erase(gamemanager.buildingLists[targetedUnit->gameObject->tag].begin() + i);
						targetedUnit->gameObject->tag = gameObject->tag;
						gamemanager.buildingLists[gameObject->tag].push_back(targetedUnit);
						UnitOrders.erase(UnitOrders.begin());
						break;
					}
				}
				//Debug.Log("Enemy Hit!");
				actionTime = 0;
			}
		}
		else
		{
			SecondMoveCommand(&targetedUnit->gameObject->transform.getPosition());
		}
	}
}

void Unit::dieCommand()
{
	dieTime += Time.getDeltaTime();
	gameObject->unitIsAvtive = false;
	this->soundAction = 9;
	if(dieTime > 1)
		gameObject->transform.setPosition(DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(0, dieTime*0.01, 0, 0)));
	if (dieTime > 3)
	{
		//play death animation
		//Debug.Log("Play death animation here");
		dieTime = 0;
		UnitOrders.erase(UnitOrders.begin());
		if (this->getType() == Soldier || this->getType() == Worker || this->getType() == Hero)
		{
			for (int i = 0; i < gamemanager.unitLists[gameObject->tag].size(); i++)
			{
				if (this == gamemanager.unitLists[gameObject->tag][i])
				{
					gamemanager.unitLists[gameObject->tag].erase(gamemanager.unitLists[gameObject->tag].begin() + i);
				}
			}
		}
		if (this->getType() == Barrack || this->getType() == Bank || this->getType() == GoldMine)
		{
			for (int i = 0; i < gamemanager.buildingLists[gameObject->tag].size(); i++)
			{
				if (this == gamemanager.buildingLists[gameObject->tag][i])
				{
					gamemanager.buildingLists[gameObject->tag].erase(gamemanager.buildingLists[gameObject->tag].begin() + i);
				}
			}
		}
		destroyUnit();
	}
}

float Unit::getDistanceBetweenUnits(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos)
{
	DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(targetPos, unitPos);
	float tempDistance = DirectX::XMVectorGetW(DirectX::XMVector3Length(diff));
	return tempDistance;
}

DirectX::XMVECTOR Unit::calculateOffsetInPath(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos)
{
	
	DirectX::XMVECTOR vectorBetween = DirectX::XMVectorSubtract(unitPos, targetPos);
	DirectX::XMVECTOR normalizedDistance = DirectX::XMVector3Normalize(vectorBetween);
	return normalizedDistance;
}

void Unit::ReceiveOrder(RaycastHit Values, int unitTag)
{
	if (!UnitOrders.empty() && UnitOrders.at(0).command == Command::Die)
	{

	}
	else{
		UnitOrders.clear();
		pathNodes.clear();

		

		//Target is a unit
		if (Values.transform->gameObject->getComponent<Unit>() != nullptr)
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
						tempOrder.command = Move;
						tempOrder.point = Values.point;
						tempOrder.transform = Values.transform;
						UnitOrders.push_back(tempOrder);
						
					}
					break;

				case Type::Soldier:
					tempOrder.command = Move;
					tempOrder.point = Values.point;
					tempOrder.transform = Values.transform;
					UnitOrders.push_back(tempOrder);
					break;

				case Type::Worker:
					if (Values.transform->gameObject->getComponent<Unit>()->getType() == Hero || Values.transform->gameObject->getComponent<Unit>()->getType() == Soldier)
					{
						tempOrder.command = Move;
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
						
					}
					if (Values.transform->gameObject->getComponent<Unit>()->getType() == Barrack)
					{
						tempOrder.command = convertToSoldier;
						tempOrder.point = Values.point;
						tempOrder.transform = Values.transform;
						UnitOrders.push_back(tempOrder);
						actionTime = 2;
					}
					break;
				}
			}
			else if (Values.transform->gameObject->tag != unitTag && Values.transform->gameObject->tag != 3 && Values.transform->gameObject->tag != 0) { //!=gameObject->tag){
				//enemy
				switch (type)
				{
				case Type::Hero:
					if (Values.transform->gameObject->getComponent<Unit>()->getType() == Bank || Values.transform->gameObject->getComponent<Unit>()->getType() == Barrack)
					{
						tempOrder.command = takeBuilding;
						tempOrder.point = Values.point;
						tempOrder.transform = Values.transform;
						UnitOrders.push_back(tempOrder);
						actionTime = 2;
					}
					else
					{
						tempOrder.command = Attack;
						tempOrder.point = Values.point;
						tempOrder.transform = Values.transform;
						UnitOrders.push_back(tempOrder);
					
					}
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
			else if (Values.transform->gameObject->tag == 0) {
				//Terrain
				//Walk 

				switch (type)
				{
				case Type::Hero:
					if (Values.transform->gameObject->getComponent<Unit>()->getType() == GoldMine)
					{
						tempOrder.command = HeroGather;
						tempOrder.point = Values.point;
						tempOrder.transform = Values.transform;
						UnitOrders.push_back(tempOrder);
						
					}
					if (Values.transform->gameObject->getComponent<Unit>()->getType() == Bank || Values.transform->gameObject->getComponent<Unit>()->getType() == Barrack)
					{
						tempOrder.command = takeBuilding;
						tempOrder.point = Values.point;
						tempOrder.transform = Values.transform;
						UnitOrders.push_back(tempOrder);
						actionTime = 2;
					}
					break;


				case Type::Worker:
					if (Values.transform->gameObject->getComponent<Unit>()->type == GoldMine)
					{
						tempOrder.command = Gather;
						tempOrder.point = Values.point;
						tempOrder.transform = Values.transform;
						UnitOrders.push_back(tempOrder);
						
					}

					break;

				default:
					//walk to
					break;
				}
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

			case Type::GoldMine:
				break;

			case Type::Barrack:
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
}

void Unit::ReceiveOrder(OPTIONS option)
{
	if (!UnitOrders.empty() && UnitOrders.at(0).command == Command::Die)
	{

	}
	else 
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

			}
			else if (option == Option2)
			{

			}
			else if (option == Option3)
			{

			}
			break;

		case Type::Barrack:
			if (option == Option0)
			{
				tempOrder.command = SummonSoldier;
				UnitOrders.push_back(tempOrder);
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
}

void Unit::update()
{
	if (gamemanager.gameState == GAME_STATE::MAIN_MENU) return;

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

		case Command::SummonWorker: //SUMMON
			summonWorkerCommand();
			break;

		case Command::SummonSoldier:
			summonSoldierCommand();
			break;

		case Command::convertToSoldier:
		{
			Unit * targetedUnit = UnitOrders.at(0).transform->gameObject->getComponent<Unit>();
			convertToSoldierCommand(targetedUnit);
		}
		break;

		case Command::takeBuilding:
		{
			Unit * targetedUnit = UnitOrders.at(0).transform->gameObject->getComponent<Unit>();
			takeBuildingCommand(targetedUnit);
		}
		break;

		case Command::Die:
			dieCommand();
			break;

		default:
			break;
		}
		
		//if ((this->getType() == Worker || this->getType() == Soldier) && this->homePos->gameObject->unitIsAvtive == false || this->homePos != nullptr)
		//{
		//	this->setHomePos(nullptr);
		//}
		
	}
}
