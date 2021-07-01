#pragma once
#include "HighResolutionTimer.h"
#include <sstream>

#include "Window.h"

class ProcessMessage
{
	static HighResolutionTimer timer;
	MSG msg;

public:
	static LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool Process();

	MSG& GetMSG() { return msg; }
	float GetElapsedTime() { return timer.TimeInterval(); }

	void SetFramerate(float framerate) { timer.SetFrameRate(framerate); }

	void CalculateFrameStats(HWND hwnd)
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.  These stats 
		// are appended to the window caption bar.
		static int frames = 0;
		static float time_tlapsed = 0.0f;

		frames++;

		// Compute averages over one second period.
		if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
		{
			#if defined(DEBUG) | defined(_DEBUG)

			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			
			float fps = static_cast<float>(frames); // fps = frameCnt / 1
			float mspf = 1000.0f / fps;
			std::ostringstream outs;
			outs.precision(7);
			outs << Window::GetInstance().GetWindowName().c_str() << "            FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
			SetWindowTextA(hwnd, outs.str().c_str());

			#endif
			// Reset for next average.
			frames = 0;
			time_tlapsed += 1.0f;
		}
	}

};