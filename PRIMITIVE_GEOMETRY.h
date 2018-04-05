#pragma once

#include <DirectXMath.h>
#include "GameObject.h"

enum class PRIMITIVE_GEOMETRY
{
	OOBB, AABB, Cube, Sphere, Capsule
};

struct OOBB {
	DirectX::XMVECTOR centre;
	DirectX::XMVECTOR x_hx;
	DirectX::XMVECTOR y_hy;
	DirectX::XMVECTOR z_hz;
	
};

struct Ray {
	DirectX::XMVECTOR direction;
	DirectX::XMVECTOR origin;
};

struct RaycastHit {
	DirectX::XMVECTOR point;
	float distance;
	GameObject* gameObject;
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
