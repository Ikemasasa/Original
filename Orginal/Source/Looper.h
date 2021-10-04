#pragma once
#include "lib/ProcessMessage.h"

class Window;

class Looper final
{
private:
	ProcessMessage pm;

public:
	Looper() = default;
	~Looper() = default;

	int Run(HWND hwnd);
};