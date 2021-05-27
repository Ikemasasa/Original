#include <windows.h>
#include <minwindef.h>
#include <memory>
#include <assert.h>

#include "lib/framework.h"
#include "lib/Window.h"
#include "lib/ProcessMessage.h"

#include "Define.h"
#include "Looper.h"


INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	auto& window = Window::GetInstance();
	window.Initialize(instance, cmd_show, Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT);

	Looper looper;
	looper.SetFramerate(Define::FRAMERATE);
	return looper.Run(window.GetHWND());
}
