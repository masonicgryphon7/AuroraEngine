#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "PRIMITIVE_GEOMETRY.h"

class Camera : public Component
{

public:
	Camera();
	Camera(int height, int width, float fov, float nearZ, float farZ);

	~Camera();

	void update();
	DirectX::XMMATRIX calculateViewMatrix();
	DirectX::XMMATRIX calculatePerspectiveMatrix();
	Ray calculateScreenPointToRay(DirectX::XMVECTOR postion);
	int getWidth() { return width; };
	int getHeight() { return height; };


	int height; 
	int width;
	float FOV;
	float aspectRatio;
	float nearZ;
	float farZ;

};

