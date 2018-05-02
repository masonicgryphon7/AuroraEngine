#pragma once
#include "Component.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Time.h"
#include "InputHandler.h"
#include "Transform.h"

#include <vector>

class Unit;
class PlayerScript : public Component
{

private:
	//Time * time;
	//InputHandler* inputHandler;
	float speed = 0;
	float distance = 0;
	DirectX::XMVECTOR direction;
	DirectX::XMVECTOR position;

	float pitch;
	float yaw;
	bool firstMouse;
	float lastX, lastY;
	double xpos, ypos;
	float xoffset;
	float yoffset;
	float sensitivity;



public:
	PlayerScript();
	~PlayerScript();

	void update();

	//std::vector<GameObject*>*objectsToRender;

	std::vector<Unit*> mfriendlyUnits;
	std::vector<Unit*> friendlyBuildings;
};

