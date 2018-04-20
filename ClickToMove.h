#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "Node.h"
#include <vector>
#define EPSILON 0.00001
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
	std::vector<Node> pathNodes;
};

