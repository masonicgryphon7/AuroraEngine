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

	void Start(HWND* w, ID3D11Device* d, ID3D11DeviceContext* dc, CoreEngine* ce);
	void Update();

	GameObject* playerCamera;

protected:
	HWND * wnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	CoreEngine* coreEngine;
	JGUI* jGUI = nullptr;

	Camera* mc;
};

