#pragma once

#include <Windows.h>
#include <string>
#include <functional>

namespace Window
{
	HINSTANCE m_instance;
	HWND m_handle;
	std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> m_OnMessage;
	std::function<void(int, int)> m_OnResize;

	// Window Procedure
	inline LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		m_OnMessage(hwnd, msg, wParam, lParam);

		switch (msg)
		{
		case WM_DISPLAYCHANGE:
			m_OnResize(lParam & 0xffff, (lParam >> 16) & 0xffff);
			break;

		case WM_SIZE:
			m_OnResize(lParam & 0xffff, (lParam >> 16) & 0xffff);
			break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		return 0;
	}

	// https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
	inline std::wstring s2ws(const std::string& str)
	{
		int len;
		int slength = (int)str.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, nullptr, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}


	inline bool Create(HINSTANCE instance, const std::string& title)
	{
		m_instance = instance;
		std::wstring windowTitle = s2ws(title);
		int windowWidth = GetSystemMetrics(SM_CXSCREEN);
		int windowHeight = GetSystemMetrics(SM_CYSCREEN);
		LPCWSTR className = L"dSXOv9vbM1Mlm5kAf4yjjw==";

		// Register the Window Class
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_instance;
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = className;
		wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
		{
			MessageBox(nullptr, L"Window registration failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		// Create the Window
		m_handle = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			className,
			windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
			nullptr, nullptr, m_instance, nullptr);

		if (!m_handle)
		{
			MessageBox(nullptr, L"Window creation failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		return true;
	}

	inline void Show()
	{
		ShowWindow(m_handle, SW_MAXIMIZE);
		UpdateWindow(m_handle);
		SetFocus(m_handle);
	}

	inline bool Tick()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return msg.message != WM_QUIT;
	}

	inline int GetWidth()
	{
		RECT rect;
		GetClientRect(m_handle, &rect);
		return  (int)(rect.right - rect.left);
	}

	inline int GetHeight()
	{
		RECT rect;
		GetClientRect(m_handle, &rect);
		return (int)(rect.bottom - rect.top);
	}
}
