#pragma once
#include"Component.h"
#include <iostream>
#include <DirectXMath.h>

class Transform :public Component
{
public:
	Transform();
	Transform(DirectX::XMVECTOR otherPosition);
	~Transform();


	void update();
	void updateRotationInEuler();

	DirectX::XMVECTOR getPosition() { return position; };
	void setPosition(const DirectX::XMVECTOR in_setPosition) { position = in_setPosition; };
	
	DirectX::XMVECTOR getRotation() { return rotation; };
	void setRotation(const DirectX::XMVECTOR in_setRotation) { rotation = in_setRotation; };

	DirectX::XMVECTOR getScale() { return scale; };
	void setScale(const DirectX::XMVECTOR in_setScale) { scale = in_setScale; };


	DirectX::XMVECTOR getForward() { return forward; };
	void setForward(const DirectX::XMVECTOR in_setForward) { forward = in_setForward; };

	DirectX::XMVECTOR getUp() { return up; };
	void setUp(const DirectX::XMVECTOR in_setUp) { up = in_setUp; };

	DirectX::XMVECTOR getRight() { return right; };
	void setRight(const DirectX::XMVECTOR in_setRight) { right = in_setRight; };

private:

	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;

	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR right;

};

