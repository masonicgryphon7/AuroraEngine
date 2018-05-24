#pragma once
#include "GameManager.h"
#include"Component.h"

class Prop : public Component
{
private:
	float dieTime = 0;
	float distance = 0;

public:
	Prop();
	~Prop();

	void disappearInLava();
	float getDistanceBetweenProps(DirectX::XMVECTOR unitPos, DirectX::XMVECTOR targetPos);
	void destroy();

	void update();
};

