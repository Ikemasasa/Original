#pragma once

#include <string>

class EventData
{
public:
	enum EventID
	{
		GAME_BEGIN,
		GAME_BEGIN_END,
		BOSS_ENCOUNT,

		EVENT_NUM
	};

	static std::string GetEventFilePath(const int eventID);
};