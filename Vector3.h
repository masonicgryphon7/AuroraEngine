#pragma once

#include <string>
#include <DirectXMath.h>

class Vector3
{
public:
	Vector3() = default;
	Vector3(float x_in, float y_in, float z_in);
	Vector3(DirectX::XMVECTOR in);
	Vector3 operator+ (const Vector3& vec3) const;
	bool operator!= (const Vector3& vec3) const;
	bool operator== (const Vector3& vec3) const;

	DirectX::XMVECTOR asXMVECTOR();

	const std::string toString() const;

	float x, y, z;
};
