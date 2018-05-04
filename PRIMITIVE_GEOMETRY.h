#pragma once

#include <DirectXMath.h>
#include "Transform.h"
#include <vector>
#include <string>
enum class Primitives
{
	OOBB,
	AABB,
	Cube,
	Sphere,
	Capsule,
	Cylinder,
	Empty
};

enum OPTIONS
{
	Option0,
	Option1,
	Option2,
	Option3
};

struct Joint {
	std::string name;
	unsigned int parentID;
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;
};

struct AnimationFrame {
	std::vector<Joint> joints;
};


struct Skeleton {
	std::vector < DirectX::XMMATRIX > bindPose;
	std::vector < DirectX::XMMATRIX > inverseBindPose;

};


enum OPTIONS
{
	Option0,
	Option1,
	Option2,
	Option3
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

struct VERTEX_POS3UV2T3B3N3JNT4WT4 {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 jointIndex;
	DirectX::XMFLOAT4 jointWeight;
};

struct VERTEX_POS3UV2
{
	DirectX::XMFLOAT3  position;
	DirectX::XMFLOAT2 uv;
};
