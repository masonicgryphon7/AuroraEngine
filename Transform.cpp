#include "Transform.h"



Transform::Transform()
{
	position = DirectX::XMVectorSet(0, 0, 0, 0);
	rotation = DirectX::XMVectorSet(0, 0, 0, 0);
	scale = DirectX::XMVectorSet(1, 1, 1, 0);

	forward = DirectX::XMVectorSet(1, 0, 0, 0);
	up = DirectX::XMVectorSet(0, 1, 0, 0);
	right = DirectX::XMVectorSet(0, 0, 1, 0);
}

Transform::Transform(DirectX::XMVECTOR otherPosition)
{

	position=otherPosition;
	rotation=DirectX::XMVectorSet(0,0,0,0);
	scale=DirectX::XMVectorSet(1, 1, 1, 0);

	forward = DirectX::XMVectorSet(1, 0, 0, 0);
	up = DirectX::XMVectorSet(0, 1, 0, 0);
	right = DirectX::XMVectorSet(0, 0, 1, 0);
}


Transform::~Transform()
{
}

void Transform::update()
{
}

void Transform::updateRotationInEuler()
{
}
