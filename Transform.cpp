#include "Transform.h"



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

DirectX::XMVECTOR Transform::getRotationEuler()
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	
	MatrixToEuler(&x, &y, &z, DirectX::XMMatrixRotationQuaternion(rotation));
	//x *= 180 / PI;
	//y *= 180 / PI;
	//z *= 180 / PI;

	return DirectX::XMVectorSet(x, y, z, 0.0f);
	
}

void Transform::MatrixToEuler(float * outX, float * outY, float * outZ, DirectX::XMMATRIX rotMatrix)
{
	DirectX::XMFLOAT3X3 matrix;
	DirectX::XMStoreFloat3x3(&matrix, rotMatrix);
	*outX = atan2(matrix._21, matrix._22);
	*outY = atan2(-matrix._31, sqrt(pow(matrix._32, 2) + pow(matrix._33, 2)));
	*outZ = atan2(matrix._21, matrix._11);


	
}
