#include "NPC.h"



NPC::NPC(std::vector<Unit*> *player_units, std::vector<Unit*> *player_buildings) : Component(-1, "NPC")
{
	this->player_units = player_units;
	this->player_buildings = player_buildings;
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
	enemy_unit->addComponent(new MaterialFilter(AssetManager.getMaterial("UnitMaterial")));
	Unit* enemy_unit_hero = new Unit(Hero);
	enemy_unit->addComponent(enemy_unit_hero);
	npc_units.push_back(enemy_unit_hero);
	this->npc_units[0]->setResources(0);
	gamemanager.unitLists[this->npc_units[0]->gameObject->tag].push_back(this->npc_units[0]);
	
}

void NPC::findNewEnemies(Unit* unit)
{
	float temp_distance;
	for (int i = 0; i < player_units->size(); i++)
	{
		
			temp_distance = unit->getDistanceBetweenUnits(unit->gameObject->transform.getPosition(), player_units[0][i]->gameObject->transform.getPosition());
			
			
			if (temp_distance < unit->getUnitDistance() || unit->getUnitDistance() == 0)
			{
				unit->setDistance(temp_distance);
				unit->setTargetPos(player_units[0][i]->gameObject->transform.getPosition());
				RaycastHit hit;
				hit.transform = &player_units[0][i]->gameObject->transform;
				unit->ReceiveOrder(hit, unit->gameObject->tag);
			}

		
	}
}

void NPC::attack(Unit * unit)
{
	float temp_distance;
	for (int i = 0; i < player_units->size(); i++)
	{

		temp_distance = unit->getDistanceBetweenUnits(unit->gameObject->transform.getPosition(), player_units[0][i]->gameObject->transform.getPosition());


		if (temp_distance < unit->getUnitDistance() || unit->getUnitDistance() == 0)
		{
			unit->setDistance(temp_distance);
			unit->setTargetPos(player_units[0][i]->gameObject->transform.getPosition());
			RaycastHit hit;
			hit.transform = &player_units[0][i]->gameObject->transform;
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
	//GameObject* worker = gScene.createEmptyGameObject(gameObject->transform.getPosition());//gScene.createEmptyGameObject(DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(1.0, 0.0, -3.0, 0.0)));
	//worker->name = "Worker"; //+ playerScript->friendlyUnits.size();
	//worker->tag = gameObject->tag;
	//MeshFilter* meshFilter = new MeshFilter(AssetManager.getMesh("pose1smile"));
	//worker->addComponent(meshFilter);
	//worker->addComponent(new MaterialFilter(AssetManager.getMaterial("UnitMaterial")));
	//Unit *unitWorker = new Unit(Worker);
	//unitWorker->setHomePos(&building->gameObject->transform);
	//worker->addComponent(unitWorker);
	//npc_units.push_back(unitWorker);
	//

	//UnitOrders.erase(UnitOrders.begin());
	//Order tempOrder;
	//tempOrder.command = Move;
	//tempOrder.point = DirectX::XMVectorSubtract(gameObject->transform.getPosition(), DirectX::XMVectorSet(1.0, 0.0, -3.0, 0.0));//DirectX::XMVectorSet(1.0, 0.0, 3.0, 0.0);
	//unitWorker->getUnitOrdersPointer()->push_back(tempOrder);
}



