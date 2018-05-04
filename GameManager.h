#pragma once
#include"Component.h"
#include <DirectXMath.h>
#include "Transform.h"
#include "Debug.h"
#include "Time.h"
#include <d3d11.h>
#include <d3dcompiler.h>
class GameManager : public Component
{
public:
	GameManager();
	~GameManager();
	float ringOfFire;
	float gameTime;
	float getGameTime() { return this->gameTime; };
	float getRingRadius() { return this->ringOfFire; };

	void setResources(float radius) { this->ringOfFire = radius; };
	void update();
};

