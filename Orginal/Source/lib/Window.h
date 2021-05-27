#pragma once
#include <windows.h>
#include <tchar.h>

class Window
{
	const LPCWSTR className = _T("Original");
	HWND hwnd;
	float width;
	float height;

private:
	void Create(HINSTANCE instance, int cmd_show, float width, float height);

public:
	Window() : hwnd(0), width(0), height(0) {}
	~Window() = default;

	void Initialize(HINSTANCE instance, int cmd_show, float width/*ウィンドウ横サイズ*/, float height/*ウィンドウ縦サイズ*/);
	HWND& GetHWND() { return hwnd; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }

	static Window& GetInstance()
	{
		static Window instance;
		return instance;
	}
};

#define WINDOW (Window::GetInstance())