#include "InputHandler.h"
#include "Console.h"

Vector2 InputHandler::viewportSize;
Vector2 InputHandler::mousePosition;
Vector2 InputHandler::fullscreenMousePosition;

InputHandler::InputHandler()
{}


InputHandler::InputHandler(HWND wndHandle)
{
	wnd = FindWindow(L"dSXOv9vbM1MIm5kAf4yjjw", 0); //set encrypted Aurora
	//keyboard = new DirectX::Keyboard();
	mouse = new DirectX::Mouse();
	mouse->SetWindow(wndHandle);
}

InputHandler::~InputHandler()
{
	delete mouse;
}

void InputHandler::updateInput()
{
	for (unsigned int k = 0U; k < 256U; ++k)
	{
		previousKeys[k] = detectKey(k);
	}

	/*kb = keyboard->GetState();
	kbTracker.Update(kb);*/

	mouseState = mouse->GetState();
}

bool InputHandler::GetKey(KeyCode key)
{
	//return keyStates[(int)key] == KeyState::Held;
	return (GetAsyncKeyState((unsigned int)key) & (1 << (sizeof(SHORT) * 8 - 1)));
}

bool InputHandler::GetKeyDown(KeyCode key)
{
	//return keyStates[(int)key] == KeyState::Down;

	int intKey = (unsigned int)key;

	bool previousState = previousKeys[intKey];

	previousKeys[intKey] = GetKey(key);

	return (previousKeys[intKey] && !previousState);
}

bool InputHandler::GetKeyUp(KeyCode key)
{
	//return keyStates[(int)key] == KeyState::Up;

	int intKey = (unsigned int)key;

	bool previousState = previousKeys[intKey];

	previousKeys[intKey] = GetKey(key);

	return (!previousKeys[intKey] && previousState);
}

/// <summary>
///  <para> Gets the position of the entire screen </para>
/// </summary>
POINT InputHandler::GetMouseCoordinates()
{
	POINT point;
	GetCursorPos(&point);
	return point;
}

/// <summary>
///  <para> Gets the position of the engine screen </para>
/// </summary>
POINT InputHandler::GetAbsoluteMouseCoordinates()
{
	if (wnd == NULL)
		wnd = FindWindow(L"dSXOv9vbM1MIm5kAf4yjjw", 0);

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(wnd, &point);

	Vector2 screen = GetEngineWindowResolution();

	if (point.x < 0)
		point.x = 0;
	else if (point.x > screen.x)
		point.x = screen.x;

	if (point.y < 0)
		point.y = 0;
	else if (point.y > screen.y)
		point.y = screen.y;

	return point;
}

Vector2 InputHandler::GetMousePosition()
{
	return Vector2(GetAbsoluteMouseCoordinates().x - currentFramePos.x, GetAbsoluteMouseCoordinates().y - currentFramePos.y);
}

Vector2 InputHandler::GetDesktopResolution()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	return Vector2(desktop.right, desktop.bottom);
}

Vector2 InputHandler::GetEngineWindowResolution()
{
	if (wnd == NULL)
		wnd = FindWindow(L"dSXOv9vbM1MIm5kAf4yjjw", 0);

	RECT rect;

	int width = 0, height = 0;

	if (GetWindowRect(wnd, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		//width -= 16;
		//height -= 39;
	}
	return Vector2(width, height);
}

Vector2 InputHandler::GetViewportSize()
{
	return viewportSize;
}

void InputHandler::InternalSetMouseViewport(unsigned int x, unsigned int y)
{
	Vector2 cursorPos = Vector2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
	Vector2 windowPos = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
	currentFramePos = Vector2((cursorPos.x + windowPos.x), (cursorPos.y + windowPos.y));
	viewportSize = Vector2(x, y);
}

bool InputHandler::detectKey(int key)
{
	return (GetAsyncKeyState(key) & (1 << (sizeof(SHORT) * 8 - 1)));
}
