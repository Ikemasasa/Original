#pragma once

#include <windows.h>
#include <timeapi.h>

class HighResolutionTimer
{
	LONGLONG mBaseTime;
	LONGLONG mTickTime[2]; // 0:現在, 1:前フレーム
	LONGLONG mStopTime;
	LONGLONG mPausedTime;
	LONGLONG mOneFrameTime;

	double mElapsedTime;
	double mSecondsPerCount;
	bool mIsStopped;

public:
	void Initialize();

	void Stop();
	void Start();

	float TimeStamp() const;
	bool Tick();

	float GetElapsedTime();
};
