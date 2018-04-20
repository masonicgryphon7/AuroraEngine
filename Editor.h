#pragma once

#include <vector>
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "CoreEngine.h"

struct ImGuiContext;
class GUI;

class Editor
{
public:
	Editor();
	~Editor();

	void Start(HWND* w, ID3D11Device* d, ID3D11DeviceContext* dc, CoreEngine* ce);
	void Update();
	void Draw();
	void Exit();

	void OnResize();

	void SetGUIStyle();

	GameObject* editorCamera;

protected:
	HWND* wnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	CoreEngine* coreEngine;

	Camera* mc;
	EditorMoveScript* ems;
	EditorSceneSelectionScript* essc;

	std::vector<std::unique_ptr<GUI>> m_gui;

	bool isPlaying = false, onceChangeState = false;
};

