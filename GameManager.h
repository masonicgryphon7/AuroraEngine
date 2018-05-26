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
#include "NPC.h"
#include "Prop.h"
enum GAME_STATE
{
	MAIN_MENU,
	START_STATE,
	PAUSE_STATE,
	LARGE_CIRCLE_STATE, 
	MEDIUM_CIRCLE_STATE, 
	SMALL_CIRCLE_STATE,
	END_STATE,
	GAME_OVER_MENU,
	WIN_MENU
};
enum RING_STATE
{
	NO_MOVE, MOVE//, SECOND_MOVE, THIRD_MOVE
};

class Unit;
static class GameManager
{
public:
	GameManager();
	GameManager(ID3D11Device* gDevice, ID3D11DeviceContext*  gDeviceContext);
	~GameManager();
	static float ringOfFire;
	DirectX::XMVECTOR middlePoint = DirectX::XMVectorSet(149, 0.0, 149, 0.0);

	float uvPanning = 0;
	DirectX::XMFLOAT4 fireBufferData;
	float lavaSpeed = 0.04;
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
	void winCondition();
	void setResources(float radius) { this->ringOfFire = radius; };
	void update();

	static GAME_STATE gameState;
	static RING_STATE ringState;
	GAME_STATE getGameState() { return this->gameState; };
	static std::vector<std::vector<Unit*>> unitLists;
	static std::vector<std::vector<Unit*>> buildingLists;
	
	Unit* unit;

	void addBuildings();

} gamemanager;
//Ovanför

