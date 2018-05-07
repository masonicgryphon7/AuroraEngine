#include "NPC.h"



NPC::NPC(std::vector<Unit*> *player_units, std::vector<Unit*> *player_buildings) : Component(-1, "NPC")
{
	this->player_units = player_units;
	this->player_buildings = player_buildings;
}

NPC::~NPC()
{
}

void NPC::update()
{
	findNewEnemies();

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
	enemy_units.push_back(enemy_unit_hero);
	
	
}

void NPC::findNewEnemies()
{
	float temp_distance;
	for (int i = 0; i < player_units->size(); i++)
	{
		for (int j = 0; j < enemy_units.size(); j++)
		{
			temp_distance = enemy_units[j]->getDistanceBetweenUnits(enemy_units[j]->gameObject->transform.getPosition(), player_units[0][i]->gameObject->transform.getPosition());
			
			
			if (temp_distance < enemy_units[j]->getUnitDistance() || enemy_units[j]->getUnitDistance() == 0)
			{
				enemy_units[j]->setDistance(temp_distance);
				enemy_units[j]->setTargetPos(player_units[0][i]->gameObject->transform.getPosition());
				RaycastHit hit;
				hit.transform = &player_units[0][i]->gameObject->transform;
				enemy_units[j]->RecieveOrder(hit, enemy_units[j]->gameObject->tag);
			}

		}
	}
}

void NPC::attack()
{
	for (int i = 0; i < this->enemy_units.size(); i++)
	{
		
	}
}


