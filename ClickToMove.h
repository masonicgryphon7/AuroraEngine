#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Camera.h"

class ClickToMove :
	public Component
{
public:
	ClickToMove();
	ClickToMove(Camera* editorCamera);
	~ClickToMove();

	void update();

private:
	Camera* editorCamera;
	DirectX::XMFLOAT3 goalPos;
	float lerpValue;
};

