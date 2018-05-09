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
	for (int i = 0; i < gamemanager.unitLists[2].size(); i++)
	{
		if (gamemanager.unitLists[2][i]->getHealthPoints() <= 0 && gamemanager.unitLists[2][i]->getUnitOrders().size() <= 0)
		{
			//gamemanager.unitLists[2][i]->dieCommand();
			gamemanager.unitLists[2][i]->destroyUnit();
			gamemanager.unitLists[2].erase(gamemanager.unitLists[2].begin() + i);
		}
	}

	/*if (gamemanager.unitLists[2][0]->getResources() >= 100 && gamemanager.unitLists[2][0]->getUnitCommand() == HeroGather)
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
	GameObject* enemy_unit = gScene.createEmptyGameObject(DirectX::XMVectorSet(50, 0, 50, 0));
	enemy_unit->name = "ENEMY";
	enemy_unit->tag = 2;
	AssetManager.addMeshFromBinary("Assets/COLLECTOR_Mesh.bin", AssetManager.getShaderProgram("Vertex.hlsl"));

	MeshFilter* enemy_unit_meshFilter = new MeshFilter(AssetManager.getMesh("COLLECTOR_Mesh"));
	enemy_unit->addComponent(enemy_unit_meshFilter);
	enemy_unit->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));
	Unit* enemy_unit_hero = new Unit(Hero);
	enemy_unit->addComponent(enemy_unit_hero);
	//npc_units.push_back(enemy_unit_hero);
	//this->npc_units[0]->setResources(0);
	gamemanager.unitLists[enemy_unit->tag].push_back(enemy_unit_hero);
	
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
	/*for (int i = 0; i < player_buildings->size(); i++)
	{
		if (player_buildings[0][i]->getType() == GoldMine)
		{
			unit->setTargetPos(player_buildings[0][i]->gameObject->transform.getPosition());
			RaycastHit hit;
			hit.transform = &player_buildings[0][i]->gameObject->transform;
			unit->ReceiveOrder(hit, unit->gameObject->tag);
		}
	}*/
	
}

void NPC::summonWorker(Unit * building)
{
	GameObject* worker = gScene.createEmptyGameObject(gameObject->transform.getPosition());//gScene.createEmptyGameObject(DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(1.0, 0.0, -3.0, 0.0)));
	worker->name = "Worker"; //+ playerScript->friendlyUnits.size();
	worker->tag = gameObject->tag;
	MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh("pose1smile"));
	worker->addComponent(meshFilter);
	worker->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));
	Unit *unitWorker = new Unit(Worker);
	unitWorker->setHomePos(&building->gameObject->transform);
	worker->addComponent(unitWorker);
	//npc_units.push_back(unitWorker);
	
	gamemanager.unitLists[2].push_back(unitWorker);


	building->clearUnitOrder();
	
	float tempDistance = 100;
	int temp;
	if (gamemanager.buildingLists[2].size() > 0)
	{
		for (int i = 0; i < gamemanager.buildingLists[2].size(); i++)
		{
			float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(worker->transform.getPosition(), gamemanager.buildingLists[2][i]->gameObject->transform.getPosition())));

			if (distance < tempDistance)
			{
				gamemanager.unitLists[2][gamemanager.unitLists[2].size()]->setDistance(distance);
				temp = i;
			}

		}
	}
	
	gamemanager.unitLists[2][gamemanager.unitLists[2].size()]->gatherCommand(gamemanager.buildingLists[2][temp]);
//	tempOrder.point = DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(1.0, 0.0, -3.0, 0.0));//DirectX::XMVectorSet(1.0, 0.0, 3.0, 0.0);
	
}



