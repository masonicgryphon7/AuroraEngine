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

				if (nrOfSoldiers >= 3 && nrOfSoldiers > gamemanager.unitLists[1].size())
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
					else if (gamemanager.unitLists[2][0]->getResources() < 100)
					{
						gather(gamemanager.unitLists[2][0]);
					}
				}
			}
			if (gamemanager.unitLists[2][0]->getUnitOrdersSize() > 0 && gamemanager.unitLists[2][0]->getResources() >= 100)
			{
				gamemanager.unitLists[2][0]->clearUnitOrder();
				gamemanager.unitLists[2][0]->setResources(0);
			}

			if (gamemanager.unitLists[2][0]->getResources() > 20 && gamemanager.buildingLists[2].size() > 0)// && tempBool == false)
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

				if (nrOfWorkers < 2)
				{
					for (int i = 0; i < gamemanager.buildingLists[2].size(); i++)
					{
						if (gamemanager.buildingLists[2][i]->getType() == Bank)
						{
							gamemanager.unitLists[2][0]->setResources(gamemanager.unitLists[2][0]->getResources() - 40);
							summonUnit(gamemanager.buildingLists[2][i]);
							tempBool = true;
						}
					}
				}
				if (nrOfWorkers >= 2 && nrOfSoldiers <= 3)
				{
					for (int i = 0; i < gamemanager.buildingLists[2].size(); i++)
					{
						if (gamemanager.buildingLists[2][i]->getType() == Barrack)
						{
							gamemanager.unitLists[2][0]->setResources(gamemanager.unitLists[2][0]->getResources() - 40);
							summonUnit(gamemanager.buildingLists[2][i]);
							tempBool = true;
						}
					}
				}
			}

			for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
			{
				if (gamemanager.unitLists[2][i]->getType() == Worker)
				{
					if (gamemanager.unitLists[2][i]->getUnitOrders().size() == 0) //&& nrOfWorkers < 4)
					{
						gather(gamemanager.unitLists[2][i]);
					}
				}
			}
		}
	}



	/*for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
	{
		if (gamemanager.unitLists[2][i]->getHealthPoints() <= 0 && gamemanager.unitLists[2][i]->getUnitOrders().size() <= 0)
		{
			//gamemanager.unitLists[2][i]->dieCommand();
			gamemanager.unitLists[2][i]->destroyUnit();
			gamemanager.unitLists[2].erase(gamemanager.unitLists[2].begin() + i);
		}
	}

	if (gamemanager.unitLists[2][0]->getResources() >= 100 && gamemanager.unitLists[2][0]->getUnitCommand() == HeroGather)
	{
		wantsToAttackHero = true;
		gamemanager.unitLists[2][0]->clearUnitOrder();
	}
		
	for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
	{
		if (gamemanager.unitLists[2][i]->getUnitOrdersSize() < 1)
		{

			
			if (wantsToAttackHero)
			{
				attack(gamemanager.unitLists[2][i]);
			}
			else
			{
				standAbout(gamemanager.unitLists[2][i]);

			}
		}
	}*/
	

	//CODE GOES HERE

}

void NPC::instantiate_NPC()
{
	GameObject* enemy_unit = gScene.createEmptyGameObject(DirectX::XMVectorSet(250, 0, 250, 0));
	enemy_unit->name = "ENEMY";
	enemy_unit->tag = 2;

	MeshFilter* enemy_unit_meshFilter = new MeshFilter(AssetManager.getMesh("COLLECTOR_Mesh"));
	enemy_unit->addComponent(enemy_unit_meshFilter);
	enemy_unit->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));
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
		if (gamemanager.buildingLists[0][i]->getType() == GoldMine)
		{
			int temp = unitToUse->getDistanceBetweenUnits(unitPos, gamemanager.buildingLists[0][i]->gameObject->transform.getPosition());
			if (temp < findClosest)
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

	for (int j = 0; j < 1; j++)
	{
		for (int i = 0; i < gamemanager.buildingLists[j].size(); i++)
		{
			if (gamemanager.buildingLists[j][i]->getType() != GoldMine) //Bank
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



