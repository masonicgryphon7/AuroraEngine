#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include "CoreEngine.h"
#include "JGUI.h"

class Player
{
public:
	Player();
	~Player();

	struct HansLillaGrej
	{
		Vector3 originalPos;
		int offset;
		float timer = 1.0f;
		ImColor col = ImColor(255, 255, 255, 255);

		HansLillaGrej(int off, Vector3 p)
		{
			this->offset = off;
			this->originalPos = p;
		}
	};


	void Start(HWND* w, ID3D11Device* d, ID3D11DeviceContext* dc, CoreEngine* ce);
	void Update();

	GameObject* playerCamera;

protected:
	HWND * wnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	CoreEngine* coreEngine;
	JGUI* jGUI = nullptr;

	GameObject* enemyObject = nullptr;
	NPC* enemyNPC = nullptr;

	GAME_STATE previousState;

	Camera* mc;
};

