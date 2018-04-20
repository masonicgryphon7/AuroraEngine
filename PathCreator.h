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

#define MAX 100
#define MIN 0
#define GRID 100 // spannet mellan min till max

static class cPathCreator
{
public:
	cPathCreator();
	~cPathCreator();

	void createNodes(std::vector<std::vector<VERTEX_POS3UV2T3B3N3>> positions);
	std::vector<Node> getPath(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 goalPos);

private:
	static std::vector<std::vector<Node>> grid;
	int xArr[9] = { -1,-1,-1,0,0,0,1,1,1 };
	int zArr[9] = { -1,0,1,-1,0,1,-1,0,1 };

	float manhattan(Node first, Node second);
} PathCreator;

