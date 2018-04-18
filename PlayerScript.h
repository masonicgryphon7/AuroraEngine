#pragma once
#include "Component.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Time.h"
#include "InputHandler.h"
#include "Transform.h"

class PlayerScript : public Component
{
public:
	PlayerScript();
	~PlayerScript();

	void update();

private:
	float speed = 0;
	float distance = 0;
	DirectX::XMVECTOR direction;

	float pitch;
	float yaw;
	bool firstMouse;
	float lastX, lastY;
	double xpos, ypos;
	float xoffset;
	float yoffset;
	float sensitivity;
};

