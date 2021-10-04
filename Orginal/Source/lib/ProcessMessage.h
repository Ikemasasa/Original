#pragma once
#include "HighResolutionTimer.h"
#include <sstream>

#include "Window.h"

class ProcessMessage
{
	static HighResolutionTimer mTimer;
	MSG msg;

	int mFps = 60;

public:
	ProcessMessage();
	~ProcessMessage();

	static LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool IsLoop();
	bool IsProcess();

	void CalcFrame(HWND hwnd);

	MSG& GetMSG() { return msg; }
	float GetElapsedTime() { return mTimer.GetElapsedTime(); }
	int GetFPS() const { return mFps; }

};