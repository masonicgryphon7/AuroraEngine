#pragma once
#include"Component.h"
#include <DirectXMath.h>
#include "Transform.h"
#include "Debug.h"
#include "Time.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "CoreEngine.h"
#include "Unit.h"

enum GAME_STATE
{
	MAIN_MENU,START_STATE,LARGE_CIRCEL_STATE, MEDIUM_CIRCEL_STATE, SMALL_CIRCEL_STATE,END_STATE,GAME_OVER_MENU
};
class Unit;
static class GameManager
{
public:
	GameManager();
	GameManager(ID3D11Device* gDevice, ID3D11DeviceContext*  gDeviceContext);
	~GameManager();
	float ringOfFire;
	DirectX::XMVECTOR middlePoint = DirectX::XMVectorSet(99.0, 0.0, 99.0, 0.0);

	float gameTime;
	float getGameTime() { return this->gameTime; };
	float getRingRadius() { return this->ringOfFire; };
	void dmgRing();

	ID3D11Buffer * GameManagerBuffer;
	ID3D11DeviceContext * devCon = nullptr;
	ID3D11Device* dev = nullptr;

	struct Manager_Buffer {
		DirectX::XMVECTOR fireRing;
	};
	HRESULT createBuffer(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	void setResources(float radius) { this->ringOfFire = radius; };
	void update();

	static GAME_STATE gameState;
	static std::vector<std::vector<Unit*>> unitLists;
	Unit* unit;

} gamemanager;
//Ovanf�r

