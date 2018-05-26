#include "NPC.h"



NPC::NPC() : Component(-1, "NPC")
{
	wantsToAttackHero = false;

}

NPC::~NPC()
{
}

void NPC::update()
{
	if (gamemanager.gameState == GAME_STATE::MAIN_MENU) return;

	if (checkTime == true)
	{
		waitTime = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
		if (waitTime > 10000)
		{
			checkTime = false;
			//Debug.Log("WAIT IS OVER");
		}

	}

	if (checkTime == false) // waited for game to begin
	{
		if (gamemanager.unitLists[2].size() > 0)
		{
			if (gamemanager.unitLists[2][0]->getUnitOrders().size() == 0)
			{
				int nrOfSoldiers = 0;
				for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
				{
					if (gamemanager.unitLists[2][i]->getType() == Soldier)
					{
						nrOfSoldiers++;
					}
				}

				if ((nrOfSoldiers >= 10 && nrOfSoldiers > gamemanager.unitLists[1].size()) || gamemanager.gameState == SMALL_CIRCLE_STATE)
				{
					attack(gamemanager.unitLists[2][0]);

					for (int i = 1; i < gamemanager.unitLists[2].size(); i++)
					{
						if (gamemanager.unitLists[2][i]->getType() == Soldier)
						{
							attack(gamemanager.unitLists[2][i]);
						}
					}
				}
				else
				{
					if (gamemanager.buildingLists[2].size() < 4)//gamemanager.unitLists[2][0]->getResources() <= 0)
					{
						takeOverBuilding(gamemanager.unitLists[2][0]);
					}
				}
			}
			
			//if (gamemanager.unitLists[2][0]->getSoundAction() == 3)
			//{
			//	for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
			//	{
			//		if (gamemanager.unitLists[2][i]->getType() == Soldier)
			//		{
			//			Order tempOrder;
			//			tempOrder.point = gamemanager.unitLists[2][0]->gameObject->transform.getPosition();
			//			tempOrder.command = Move;
			//		}
			//	}
			//}

			for (int i = 0; i < gamemanager.unitLists[1].size(); i++)
			{
				if (gamemanager.unitLists[1][i]->getUnitCommand() == Command::Attack)
				{
					for (int j = 0; j < gamemanager.unitLists[2].size(); j++)
					{
						if (gamemanager.unitLists[2][j]->getType() == Soldier)
						{
							Order temporder;
							temporder.point = gamemanager.unitLists[1][i]->gameObject->transform.getPosition();
							temporder.transform = &gamemanager.unitLists[1][i]->gameObject->transform;
							temporder.command = Command::Attack;
							gamemanager.unitLists[2][j]->setUnitOrder(temporder);
						}
					}
				}
			}

			if (gamemanager.unitLists[2][0]->getUnitOrders().size() == 0 && gamemanager.unitLists[2][0]->getResources() < 200)
			{
				gather(gamemanager.unitLists[2][0]);
			}
			else if (gamemanager.unitLists[2][0]->getUnitOrders().size() > 0 && gamemanager.unitLists[2][0]->getUnitOrders().at(0).command == Command::HeroGather && gamemanager.unitLists[2][0]->getResources() >= 200)
			{
				gamemanager.unitLists[2][0]->clearUnitOrder();
			}

			if (gamemanager.unitLists[2][0]->getResources() > 60 && gamemanager.buildingLists[2].size() > 0)// && tempBool == false)
			{
				int nrOfWorkers = 0;
				int nrOfSoldiers = 0;

				for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
				{
					if (gamemanager.unitLists[2][i]->getType() == Worker)
					{
						nrOfWorkers++;
					}
					if (gamemanager.unitLists[2][i]->getType() == Soldier)
					{
						nrOfSoldiers++;
					}
				}

				if (nrOfWorkers < 5)
				{
					for (int i = 0; i < gamemanager.buildingLists[2].size(); i++)
					{
						if (gamemanager.buildingLists[2][i]->getType() == Bank)
						{
							summonUnit(gamemanager.buildingLists[2][i]);
							gather(gamemanager.unitLists[2][gamemanager.unitLists[2].size()-1]);
							//tempBool = true;
						}
					}
				}
				if (nrOfWorkers >= 2 && nrOfSoldiers <= 3)
				{
					for (int i = 0; i < gamemanager.buildingLists[2].size(); i++)
					{
						if (gamemanager.buildingLists[2][i]->getType() == Barrack)
						{
							summonUnit(gamemanager.buildingLists[2][i]);
							Order tempOrder;
							tempOrder.point = gamemanager.unitLists[2][0]->gameObject->transform.getPosition();
							tempOrder.command = Move;
							gamemanager.unitLists[2][gamemanager.unitLists[2].size()-1]->setUnitOrder(tempOrder);
							//tempBool = true;
						}
					}
				}
			}

			for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
			{
				bool foundBank = false;
				bool foundBarrack = false;
				if (gamemanager.unitLists[2][i]->getType() == Worker && gamemanager.unitLists[2][i]->getUnitOrders().size() == 0)
				{
	/*				if (gamemanager.gameState == GAME_STATE::MEDIUM_CIRCLE_STATE)
					{
						for (int i = 0; i < gamemanager.buildingLists[2].size(); i++)
						{
							if (gamemanager.buildingLists[2][i]->getType() == Barrack)
							{
								RaycastHit hit;
								hit.transform = &gamemanager.buildingLists[2][i]->gameObject->transform;
								gamemanager.unitLists[2][i]->ReceiveOrder(hit, gamemanager.unitLists[2][i]->gameObject->tag);
								foundBarrack = true;
							}
						}
					}*/
					//else if(foundBarrack == false)
					//{
						if (gamemanager.unitLists[2][i]->getHomePos() == nullptr)
						{
							for (int j = 0; j < gamemanager.buildingLists[2].size(); j++)
							{
								if (gamemanager.buildingLists[2][j]->getType() == Bank)
								{
									gamemanager.unitLists[2][i]->setHomePos(&gamemanager.buildingLists[2][j]->gameObject->transform);
									foundBank = true;
								}
							}
						}
						if (foundBank == true)//gamemanager.unitLists[2][i]->getHomePos() != nullptr)
							gather(gamemanager.unitLists[2][i]);
					//}
					//if (foundBank == false)
					//{
					//	gamemanager.unitLists[2][i]->setHomePos(nullptr);
					//
					//	for (int i = 0; i < gamemanager.buildingLists[2].size(); i++)
					//	{
					//		if (gamemanager.buildingLists[2][i]->getType() == Barrack)
					//		{
					//			RaycastHit hit;
					//			hit.transform = &gamemanager.buildingLists[2][i]->gameObject->transform;
					//			gamemanager.unitLists[2][i]->ReceiveOrder(hit, gamemanager.unitLists[2][i]->gameObject->tag);
					//			//foundBarrack = true;
					//		}
					//	}
					//}
				}

			}


			if (gamemanager.ringState == RING_STATE::MOVE)//&& distanceToMiddle > 150
			{
				int nrOfSoldiers = 0;

				for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
				{
					if (gamemanager.unitLists[2][i]->getType() == Soldier)
					{
						nrOfSoldiers++;
					}
					float distanceToMiddle = gamemanager.unitLists[2][i]->getDistanceBetweenUnits(gamemanager.unitLists[2][i]->gameObject->transform.getPosition(), gamemanager.middlePoint);

					if (distanceToMiddle > gamemanager.ringOfFire - 10)
					{
						DirectX::XMVECTOR pointInsideRing = DirectX::XMVectorLerp(gamemanager.unitLists[2][i]->gameObject->transform.getPosition(), gamemanager.middlePoint, 0.1);
						Order tempOrder;
						tempOrder.point = pointInsideRing;
						tempOrder.command = Move;

						gamemanager.unitLists[2][i]->setUnitOrder(tempOrder);
					}
				}

				if (nrOfSoldiers >= 2)
				{
					for (int j = 0; j < gamemanager.unitLists[2].size(); j++)
					{
						if (gamemanager.unitLists[2][j]->getType() == Soldier && harassUnits < 2)
						{
							Order temporder;
							temporder.point = gamemanager.unitLists[1][0]->gameObject->transform.getPosition();
							temporder.transform = &gamemanager.unitLists[1][0]->gameObject->transform;
							temporder.command = Command::Attack;
							gamemanager.unitLists[2][j]->setUnitOrder(temporder);
							harassUnits++;
						}
					}
				}
			}
			
			if (gamemanager.ringState == RING_STATE::NO_MOVE)
			{
				harassUnits = 0;
			}

		}
	}
}

void NPC::instantiate_NPC()
{
	GameObject* enemy_unit = gScene.createEmptyGameObject(DirectX::XMVectorSet(250, 0, 250, 0));
	enemy_unit->name = "ENEMY";
	enemy_unit->tag = 2;

	MeshFilter* enemy_unit_meshFilter = new MeshFilter(AssetManager.getMesh("Hero1"));
	enemy_unit->addComponent(enemy_unit_meshFilter);
	enemy_unit->addComponent(new MaterialFilter(AssetManager.getMaterial("HeroMaterial")));
	Unit* enemy_unit_hero = new Unit(Hero);
	enemy_unit->addComponent(enemy_unit_hero);
	//npc_units.push_back(enemy_unit_hero);
	//this->npc_units[0]->setResources(0);
	gamemanager.unitLists[enemy_unit->tag].push_back(enemy_unit_hero);
	start = std::clock();
	checkTime = true;
}

void NPC::gather(Unit* unitToUse)
{
	unitPos = unitToUse->gameObject->transform.getPosition();
	findClosest = 10000;

	DirectX::XMVECTOR orderPoint = unitToUse->gameObject->transform.getPosition();
	Transform *orderTransform = &unitToUse->gameObject->transform;

	for (int i = 0; i < gamemanager.buildingLists[0].size(); i++)
	{
		if (gamemanager.buildingLists[0][i]->getType() == GoldMine && gamemanager.buildingLists[0][i]->gameObject->unitIsAvtive == true) // || gamemanager.buildingLists[2][i]->getType() == Bank && gamemanager.buildingLists[0][i]->getResources() > 100 && gamemanager.buildingLists[0][i]->gameObject->isActive == true)
		{
			int temp = unitToUse->getDistanceBetweenUnits(unitPos, gamemanager.buildingLists[0][i]->gameObject->transform.getPosition());
			int ringToMine = unitToUse->getDistanceBetweenUnits(gamemanager.middlePoint, gamemanager.buildingLists[0][i]->gameObject->transform.getPosition());
			if (temp < findClosest && gamemanager.ringOfFire - 10 > ringToMine)
			{
				findClosest = temp;
				orderPoint = gamemanager.buildingLists[0][i]->gameObject->transform.getPosition();
				orderTransform = &gamemanager.buildingLists[0][i]->gameObject->transform;
			}
		}
	}

	unitToUse->setTargetPos(orderTransform->getPosition());
	RaycastHit hit;
	hit.transform = orderTransform;
	unitToUse->ReceiveOrder(hit, unitToUse->gameObject->tag);
}

void NPC::findNewEnemies(Unit* unit)
{
	float temp_distance;
	for (int i = 0; i < gamemanager.unitLists[1].size(); i++)
	{
		
			temp_distance = unit->getDistanceBetweenUnits(unit->gameObject->transform.getPosition(), gamemanager.unitLists[1][i]->gameObject->transform.getPosition());
			
			
			if (temp_distance < unit->getUnitDistance() || unit->getUnitDistance() == 0)
			{
				unit->setDistance(temp_distance);
				unit->setTargetPos(gamemanager.unitLists[1][i]->gameObject->transform.getPosition());
				RaycastHit hit;
				hit.transform = &gamemanager.unitLists[1][i]->gameObject->transform;
				unit->ReceiveOrder(hit, unit->gameObject->tag);
			}

		
	}
}

void NPC::attack(Unit * unit)
{
	float temp_distance;
	for (int i = 0; i <  gamemanager.unitLists[1].size(); i++)
	{

		temp_distance = unit->getDistanceBetweenUnits(unit->gameObject->transform.getPosition(), gamemanager.unitLists[1][i]->gameObject->transform.getPosition());


		if (temp_distance < unit->getUnitDistance() || unit->getUnitDistance() == 0)
		{
			unit->setDistance(temp_distance);
			unit->setTargetPos(gamemanager.unitLists[1][i]->gameObject->transform.getPosition());
			RaycastHit hit;
			hit.transform = &gamemanager.unitLists[1][i]->gameObject->transform;
			unit->ReceiveOrder(hit, unit->gameObject->tag);

		}


	}
}

void NPC::standAbout(Unit * unit)
{
	/*for (int i = 0; i < gamemanager.buildingLists[0].size(); i++)
	{
		
			unit->setTargetPos(gamemanager.buildingLists[0][i]->gameObject->transform.getPosition());
			RaycastHit hit;
			hit.transform = &gamemanager.buildingLists[0][i]->gameObject->transform;
			unit->ReceiveOrder(hit, unit->gameObject->tag);
		
	}*/
	
}

void NPC::summonUnit(Unit * building)
{
	building->ReceiveOrder(Option0);
}

void NPC::takeOverBuilding(Unit * unitToUse)
{
	unitPos = unitToUse->gameObject->transform.getPosition();

	DirectX::XMVECTOR orderPoint = unitToUse->gameObject->transform.getPosition();
	Transform *orderTransform = &unitToUse->gameObject->transform;

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < gamemanager.buildingLists[j].size(); i++)
		{
			if (gamemanager.buildingLists[j][i]->getType() != GoldMine && gamemanager.buildingLists[j][i]->gameObject != nullptr && gamemanager.buildingLists[j][i] != nullptr && gamemanager.buildingLists[j][i]->getHealthPoints() > 0)
			{
				int temp = unitToUse->getDistanceBetweenUnits(unitPos, gamemanager.buildingLists[j][i]->gameObject->transform.getPosition());
				if (temp < findClosest)
				{
					findClosest = temp;
					orderPoint = gamemanager.buildingLists[j][i]->gameObject->transform.getPosition();
					orderTransform = &gamemanager.buildingLists[j][i]->gameObject->transform;
				}
			}
		}
	}

	unitToUse->setTargetPos(orderTransform->getPosition());
	RaycastHit hit;
	hit.transform = orderTransform;
	unitToUse->ReceiveOrder(hit, unitToUse->gameObject->tag);
	findClosest = 10000;
}



