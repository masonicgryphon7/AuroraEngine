#include "NPC.h"



NPC::NPC() : Component(-1, "NPC")
{
}

NPC::~NPC()
{
}

void NPC::update()
{


	//CODE GOES HERE

}

void NPC::instantiate_NPC()
{
	GameObject* enemy_unit = gScene.createEmptyGameObject(DirectX::XMVectorSet(5, 0, 5, 0));
	enemy_unit->name = "ENEMY";
	enemy_unit->tag = 2;
	MeshFilter* enemy_unit_meshFilter = new MeshFilter(AssetManager.getMesh(4));
	enemy_unit->addComponent(enemy_unit_meshFilter);
	enemy_unit->addComponent(new MaterialFilter(AssetManager.getMaterial(2)));
	Unit* enemy_unit_hero = new Unit(Hero);
	enemy_unit->addComponent(enemy_unit_hero);
	
}


