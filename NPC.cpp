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
	if (npc_units[0]->getResources() >= 100 && npc_units[0]->getUnitCommand() == HeroGather)
	{
		wantsToAttackHero = true;
		npc_units[0]->clearUnitOrder();
	}
		
	for (int i = 0; i < npc_units.size(); i++)
	{
		if (npc_units[i]->getUnitOrdersSize() < 1)
		{

			
			if (wantsToAttackHero)
			{
				attack(npc_units[i]);
			}
			else
			{
				standAbout(npc_units[i]);

			}
		}
	}
	

	//CODE GOES HERE

}

void NPC::instantiate_NPC()
{
	GameObject* enemy_unit = gScene.createEmptyGameObject(DirectX::XMVectorSet(50, 0, 50, 0));
	enemy_unit->name = "ENEMY";
	enemy_unit->tag = 2;
	AssetManager.addMeshFromBinary("Assets/COLLECTOR.bin", AssetManager.getShaderProgram("Vertex.hlsl"));

	MeshFilter* enemy_unit_meshFilter = new MeshFilter(AssetManager.getMesh("COLLECTOR"));
	enemy_unit->addComponent(enemy_unit_meshFilter);
	enemy_unit->addComponent(new MaterialFilter(AssetManager.getMaterial("WorkerMaterial")));
	Unit* enemy_unit_hero = new Unit(Hero);
	enemy_unit->addComponent(enemy_unit_hero);
	npc_units.push_back(enemy_unit_hero);
	this->npc_units[0]->setResources(0);
	gamemanager.unitLists[this->npc_units[0]->gameObject->tag].push_back(this->npc_units[0]);
	
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
	npc_units.push_back(unitWorker);
	
	gamemanager.unitLists[2].push_back(unitWorker);


	building->clearUnitOrder();
	
	float tempDistance = 100;
	int temp;
	if (npc_buildings.size() > 0)
	{
		for (int i = 0; i < npc_buildings.size(); i++)
		{
			float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(npc_units[npc_units.size()]->gameObject->transform.getPosition(), npc_buildings[i]->gameObject->transform.getPosition())));

			if (distance < tempDistance)
			{
				npc_units[npc_units.size()]->setDistance(distance);
				temp = i;
			}

		}
	}
	
	npc_units[npc_units.size()]->gatherCommand(npc_buildings[temp]);
//	tempOrder.point = DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(1.0, 0.0, -3.0, 0.0));//DirectX::XMVectorSet(1.0, 0.0, 3.0, 0.0);
	
}



