#pragma once
#include "Component.h"
#include <DirectXMath.h>

class Camera : public Component
{

public:
	Camera();
	Camera(int height, int width, float fov, float nearZ, float farZ);

	~Camera();

	void update();
	DirectX::XMMATRIX calculateViewMatrix(DirectX::XMVECTOR position, DirectX::XMVECTOR forwardVector, DirectX::XMVECTOR upVector);
	DirectX::XMMATRIX calculatePerspectiveMatrix();
	int height;
	int width;
	float FOV;
	float aspectRatio;
	float nearZ;
	float farZ;

};

