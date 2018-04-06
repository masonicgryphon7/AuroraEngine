#include "Camera.h"
#include "Console.h"
#include "GameObject.h"

Camera::Camera()
{
}

Camera::Camera(int height, int width, float fov, float nearZ, float farZ) :Component(-1, "Camera")
{
	this->height = height;
	this->width = width;
	this->FOV = DirectX::XMConvertToRadians(fov);
	this->aspectRatio = (float)width / (float)height;
	this->nearZ = nearZ;
	this->farZ = farZ;
}


Camera::~Camera()
{
}

void Camera::update()
{
}


DirectX::XMMATRIX Camera::calculateViewMatrix(DirectX::XMVECTOR position, DirectX::XMVECTOR forwardVector, DirectX::XMVECTOR upVector)
{

	return DirectX::XMMatrixLookToLH(position, forwardVector, upVector);
}

DirectX::XMMATRIX Camera::calculatePerspectiveMatrix()
{
	/*float vfovRad = 2.0f * atan(tan(this->FOV / 2.0f) * (width / height));

	float yScale = cosf(vfovRad / 2) / sinf(vfovRad / 2);
	float xScale = yScale / aspectRatio;

	Console.print("float: ", yScale);


	return DirectX::XMMATRIX(
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, farZ / (farZ - nearZ), 1,
		0, 0, -nearZ * farZ / (farZ - nearZ), 0);*/

	return DirectX::XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);
}

Ray Camera::calculateScreenPointToRay(DirectX::XMVECTOR postion)
{
	Ray ray;
	ray.origin = gameObject->transform.getPosition();
	
	//inverse matrix
	DirectX::XMMATRIX ndcToWorld = DirectX::XMMatrixMultiply(calculateViewMatrix(gameObject->transform.getPosition(), gameObject->transform.getForward(), gameObject->transform.getUp()), calculatePerspectiveMatrix());
	ndcToWorld = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(ndcToWorld), ndcToWorld);

	//input position to ndc
	DirectX::XMVECTOR rayDir = DirectX::XMVectorSwizzle(postion, (1.0f / (float)width), (1.0f / (float)height), 1.0f, 0.0f);
	rayDir = DirectX::XMVector3Transform(rayDir, ndcToWorld);

	ray.direction = rayDir;
	return ray;
}
