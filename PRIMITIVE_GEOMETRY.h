#pragma once

#include <DirectXMath.h>
#include "Transform.h"

enum class PRIMITIVE_GEOMETRY
{
	OOBB, AABB, Cube, Sphere, Capsule
};

struct OOBB {
	bool isActive;
	DirectX::XMVECTOR centre;
	DirectX::XMVECTOR x_hx;
	DirectX::XMVECTOR y_hy;
	DirectX::XMVECTOR z_hz;

	DirectX::XMVECTOR corners[8];

};

struct Ray {
	DirectX::XMVECTOR direction;
	DirectX::XMVECTOR origin;
};

struct RaycastHit {
	DirectX::XMVECTOR point;
	float distance;
	Transform* transform;
};

struct VERTEX_POS3UV2T3B3N3 {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
	DirectX::XMFLOAT3 normal;

};

struct VERTEX_POS3UV2
{
	DirectX::XMFLOAT3  position;
	DirectX::XMFLOAT2 uv;
};
