#include "ProcessMessage.h"
#include "Window.h"

HighResolutionTimer ProcessMessage::mTimer;

ProcessMessage::ProcessMessage()
{
	mTimer.Initialize();
}

ProcessMessage::~ProcessMessage()
{

}

LRESULT ProcessMessage::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		mTimer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		mTimer.Start();
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

bool ProcessMessage::IsProcess()
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return false;
	}

	if (!mTimer.Tick()) return false;

	CalcFrame(Window::GetInstance().GetHWND());
	return true;
}

bool ProcessMessage::IsLoop()
{
	return msg.message != WM_QUIT;
}

void ProcessMessage::CalcFrame(HWND hwnd)
{
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((mTimer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		mFps = frames;

#ifdef _DEBUG
		float mspf = 1000.0f / mFps;
		std::ostringstream outs;
		outs.precision(7);
		outs << Window::GetInstance().GetWindowName().c_str() << "  |  FPS : " << mFps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(Window::GetInstance().GetHWND(), outs.str().c_str());
#endif
		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}
