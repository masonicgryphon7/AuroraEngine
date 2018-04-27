#include "Transform.h"

#include "GameObject.h"

Transform::Transform()
{
	position = DirectX::XMVectorSet(0, 0, 0, 0);
	/*rotation = */setRotation(DirectX::XMVectorSet(0, 0, 0, 0));//DirectX::XMVectorSet(0, 0, 0, 0);
	scale = DirectX::XMVectorSet(1, 1, 1, 0);

	forward = DirectX::XMVectorSet(1, 0, 0, 0);
	up = DirectX::XMVectorSet(0, 1, 0, 0);
	right = DirectX::XMVectorSet(0, 0, 1, 0);
}

Transform::Transform(DirectX::XMVECTOR otherPosition)
{

	position = otherPosition;
	setRotation(DirectX::XMVectorSet(0, 0, 0, 0));
	//rotation = DirectX::XMVectorSet(0, 0, 0, 0);
	scale = DirectX::XMVectorSet(1, 1, 1, 0);

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

DirectX::XMVECTOR Transform::getRotationQuaternion()
{
	return DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation);
}

DirectX::XMVECTOR Transform::getRotation()
{
	return DirectX::XMVectorScale(rotation, 180/PI); 
	
}

void Transform::setRotation(const DirectX::XMVECTOR in_setRotation)
{
	rotation = DirectX::XMVectorScale(in_setRotation, PI/180.0);

	DirectX::XMMATRIX rotationM;

	rotationM = DirectX::XMMatrixRotationAxis(getRight(), -DirectX::XMVectorGetZ(in_setRotation));
	rotationM = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationAxis(getUp(), -DirectX::XMVectorGetX(in_setRotation)), rotationM);

	//up = DirectX::XMVector3Transform(up, rotationM);
	right = DirectX::XMVector3Transform(right, rotationM);
	forward = DirectX::XMVector3Transform(forward, rotationM);

}

	
