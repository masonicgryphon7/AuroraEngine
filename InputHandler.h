#pragma once

#include <Windows.h>
#include "DirectXTK-master\Inc\Keyboard.h"
#include "DirectXTK-master\Inc\Mouse.h"
#include <DirectXMath.h>
#include "Vector2.h"
#include "KeyCode.h"

constexpr int keyCount = 512; // how many keys (considering we will have joystick and buttons like "Fire1" for mouse_click)

static class InputHandler
{
public:
	InputHandler();

	InputHandler(HWND wndHandle);
	~InputHandler();

	void updateInput();

	bool GetKey(KeyCode key);
	bool GetKeyDown(KeyCode key);
	bool GetKeyUp(KeyCode key);

	POINT GetMouseCoordinates();
	POINT GetAbsoluteMouseCoordinates();

	DirectX::Mouse::State GetMouseInput() { return mouseState; };
	DirectX::Keyboard::State getKyboardInput() { return kb; };
	DirectX::Keyboard::KeyboardStateTracker getKyboardTrackerInput() { return kbTracker; };

	Vector2 GetDesktopResolution();
	Vector2 GetEngineWindowResolution();
private:
	DirectX::Keyboard* keyboard;
	DirectX::Mouse* mouse;

	HWND wnd;

	DirectX::Keyboard::State kb;
	DirectX::Mouse::State mouseState;
	DirectX::Keyboard::KeyboardStateTracker kbTracker;


	bool previousKeys[256U];

	bool detectKey(int key);

} Input;

