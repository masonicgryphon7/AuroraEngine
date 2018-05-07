#pragma once
#include"Component.h"
#include <DirectXMath.h>
#include "Transform.h"
#include "Debug.h"
#include "Time.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "CoreEngine.h"

class GameManager : public Component
{
public:
	GameManager();
	GameManager(ID3D11Device* gDevice, ID3D11DeviceContext*  gDeviceContext);
	~GameManager();
	float ringOfFire;

	float gameTime;
	float getGameTime() { return this->gameTime; };
	float getRingRadius() { return this->ringOfFire; };

	ID3D11Buffer * GameManagerBuffer;
	ID3D11DeviceContext * devCon = nullptr;
	ID3D11Device* dev = nullptr;

	struct Manager_Buffer {
		DirectX::XMVECTOR fireRing;
	};

	HRESULT createBuffer(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	void setResources(float radius) { this->ringOfFire = radius; };
	void update();
};

