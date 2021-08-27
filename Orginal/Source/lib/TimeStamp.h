#pragma once
#include <chrono>

#include "Font.h"
#include "Vector.h"

class TimeStamp
{
	static std::chrono::system_clock::time_point mStart;
	static std::chrono::system_clock::time_point mEnd;
	static Font mFont;

public:
	TimeStamp() { mFont.Initialize(); }

	static void Start() { mStart = std::chrono::system_clock::now(); };
	static void End() { mEnd = std::chrono::system_clock::now(); };
	static void Render(const Vector2& pos = Vector2::ZERO);
};