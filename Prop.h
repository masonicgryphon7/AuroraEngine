#include "GameManager.h"
#include"Component.h"

#pragma once
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

