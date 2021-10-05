#include "Window.h"
#include "ProcessMessage.h"

#include "ConvertString.h"

void Window::Initialize(HINSTANCE instance, int cmd_show, float width/*ウィンドウ横サイズ*/, float height/*ウィンドウ縦サイズ*/)
{
	this->width = width;
	this->height = height;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ProcessMessage::HandleMessage;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);

	mWindowName = "Original RPG";
	Create(instance, cmd_show, width, height);
}

void Window::Create(HINSTANCE instance, int cmd_show, float width, float height)
{
	RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	std::wstring windowname = ConvertString::ConvertToWstring(mWindowName);
	hwnd = CreateWindow(className, windowname.c_str(), WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	ShowWindow(hwnd, cmd_show);
}