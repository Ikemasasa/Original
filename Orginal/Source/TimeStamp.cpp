#include "TimeStamp.h"

#include <string>

std::chrono::system_clock::time_point TimeStamp::mStart;
std::chrono::system_clock::time_point TimeStamp::mEnd;
FontValue TimeStamp::mFontValue;

void TimeStamp::Render(const Vector2& pos)
{
	float duration = std::chrono::duration_cast<std::chrono::milliseconds>(mEnd - mStart).count();
	mFontValue.RenderSet(duration, pos);
	mFontValue.Render();
}
