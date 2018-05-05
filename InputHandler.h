#pragma once

#include <Windows.h>
#include "DirectXTK-master\Inc\Keyboard.h"
#include "DirectXTK-master\Inc\Mouse.h"
#include <DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"
#include "KeyCode.h"
#include "imgui.h"

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

	static Vector2 mousePosition;
	static Vector2 fullscreenMousePosition;

	Vector2 GetMousePosition();

	// DON'T CALL
	void InternalSetMouseViewport(unsigned int x, unsigned int y);

	DirectX::Mouse::State GetMouseInput() { return mouseState; };
	DirectX::Keyboard::State getKyboardInput() { return kb; };
	DirectX::Keyboard::KeyboardStateTracker getKyboardTrackerInput() { return kbTracker; };

	Vector2 GetDesktopResolution();
	Vector2 GetEngineWindowResolution();
	Vector2 GetViewportSize();

	inline int GetWidth()
	{
		RECT rect;
		GetClientRect(wnd, &rect);
		return  (int)(rect.right - rect.left);
	}

	inline int GetHeight()
	{
		RECT rect;
		GetClientRect(wnd, &rect);
		return (int)(rect.bottom - rect.top);
	}

	inline Vector2 GetFullscreenWindow()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		return Vector2(desktop.right, desktop.bottom);
	}

private:
	DirectX::Keyboard* keyboard;
	DirectX::Mouse* mouse;

	Vector2 currentFramePos;
	static Vector2 viewportSize;

	HWND wnd;

	DirectX::Keyboard::State kb;
	DirectX::Mouse::State mouseState;
	DirectX::Keyboard::KeyboardStateTracker kbTracker;


	bool previousKeys[256U];

	bool detectKey(int key);

	POINT GetMouseCoordinates();
	POINT GetAbsoluteMouseCoordinates();
} Input;
