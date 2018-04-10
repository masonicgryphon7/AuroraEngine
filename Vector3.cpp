#include "Vector3.h"

Vector3::Vector3(float x_in, float y_in, float z_in) : x(x_in), y(y_in), z(z_in)
{}

Vector3::Vector3(DirectX::XMVECTOR in) : x(DirectX::XMVectorGetX(in)), y(DirectX::XMVectorGetY(in)), z(DirectX::XMVectorGetZ(in))
{
}

Vector3 Vector3::operator+(const Vector3 & vec3) const
{
	return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
}

bool Vector3::operator!=(const Vector3 & vec3) const
{
	if (x != vec3.x || y != vec3.y || z != vec3.z || x != vec3.x && y != vec3.y && z != vec3.z)
		return false;
	else
		return true;
}

bool Vector3::operator==(const Vector3 & vec3) const
{
	if (x == vec3.x && y == vec3.y && z == vec3.z)
		return true;
	else
		return false;
}

DirectX::XMVECTOR Vector3::asXMVECTOR()
{
	DirectX::XMVECTOR returnType = DirectX::XMVectorSet(x, y, z, 0);
	return returnType;
}

const std::string Vector3::toString() const
{
	std::string result = "Vector3(" + std::to_string(x) + ", " + std::to_string(y) + + ", " + std::to_string(z) + ")";
	return result;
}
