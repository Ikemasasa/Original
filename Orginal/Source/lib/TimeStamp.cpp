#include "TimeStamp.h"

#include <string>

std::chrono::system_clock::time_point TimeStamp::mStart;
std::chrono::system_clock::time_point TimeStamp::mEnd;
Font TimeStamp::mFont;

void TimeStamp::Render(const Vector2& pos)
{
	double duration = std::chrono::duration_cast<std::chrono::milliseconds>(mEnd - mStart).count();
	mFont.RenderSet(std::to_wstring(duration).c_str(), pos);
	mFont.Render();
}
