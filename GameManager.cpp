#include "GameManager.h"

GameManager::GameManager()
{
	ringOfFire = 150;
	gameTime = 0;
}


GameManager::~GameManager()
{
}

void GameManager::update()
{

	gameTime += Time.getDeltaTime();
	if (gameTime >= 0 && ringOfFire >= 10)
	{
		Debug.Log(ringOfFire);  
		ringOfFire -= 0.0005f * Time.getDeltaTime() * (gameTime * 50);
	}
}
