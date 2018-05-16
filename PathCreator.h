#pragma once
#include <time.h>
#include <vector>
#include<stdlib.h>
#include "Node.h"
#include"Vec3.h"
#include<iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "PRIMITIVE_GEOMETRY.h"
#include <algorithm>
#include <thread>
#include <time.h>
#include <ctime>
#include <queue>
/*#define MAX 200
#define MIN 0
#define GRID 200*/ // spannet mellan min till max
#define EPSILON 0.00001
static class cPathCreator
{
public:
	cPathCreator();
	cPathCreator(int Width, int Height);
	~cPathCreator();

	void trumpTheBorders();
	void addTerrain(std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> positions, int Width, int Height);
	void blockGrid(DirectX::XMFLOAT3 pos);
	std::vector<Node> getPath(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 goalPos);

	void loadBlockMap();


	std::vector<Node> getRestOfPath( DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 goalPos);

private:
	static int MAX;
	static int MIN;

	static std::vector<std::vector<Node>> grid;
	int xArr[9] = { -1,-1,-1,0,0,0,1,1,1 };
	int zArr[9] = { -1,0,1,-1,0,1,-1,0,1 };

	float manhattan(Node first, Node second);
	DirectX::XMFLOAT3 getShortergGoal(const DirectX::XMFLOAT3 A, const DirectX::XMFLOAT3 B)
	{
		DirectX::XMFLOAT3 temp;
		temp.x = std::round(B.x )- std::round(A.x);
		temp.y = std::round(B.y) - std::round(A.y);
		temp.z = std::round(B.z) - std::round(A.z);

		DirectX::XMVECTOR temp_functioning_vector = DirectX::XMVectorSet(temp.x, temp.y, temp.z, 1.0);

		DirectX::XMStoreFloat3(&temp, DirectX::XMVector3Normalize(temp_functioning_vector));
		float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(temp_functioning_vector));

		if (dist > 24)
			(dist = 24);
			temp.x = A.x + temp.x * dist;
		temp.y = A.y + temp.y * dist;
		temp.z = A.z + temp.z * dist;

		return temp;
	}
} PathCreator;

