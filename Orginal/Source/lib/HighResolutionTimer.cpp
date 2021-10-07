#include "HighResolutionTimer.h"

void HighResolutionTimer::Initialize()
{
	mElapsedTime = 0.0f;
	mPausedTime = 0;
	mIsStopped = false;
	
	// 周波数
	LONGLONG countsPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
	mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
	mOneFrameTime = (LONGLONG)(countsPerSec / 60);

	// 事前準備
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mTickTime[0]));
	mBaseTime = mTickTime[0];
	mTickTime[1] = mTickTime[0];
}

void HighResolutionTimer::Stop()
{
	if (!mIsStopped)
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mStopTime));
		mIsStopped = true;
	}
}

void HighResolutionTimer::Start()
{
	LONGLONG startTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

	if (mIsStopped)
	{
		mPausedTime += (startTime - mStopTime);
		mTickTime[1] = startTime;
		mStopTime = 0;
		mIsStopped = false;
	}
}

float HighResolutionTimer::TimeStamp() const
{
	float ret = 0.0f;
	if (mIsStopped) ret = static_cast<float>(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	else			ret = static_cast<float>(((mTickTime[0] - mPausedTime) - mBaseTime) * mSecondsPerCount);

	return ret;
}

bool HighResolutionTimer::Tick()
{
	if (mIsStopped)
	{
		mElapsedTime = 0.0f;
		return false;
	}

	// 現在の時間取得
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mTickTime[0]));

	// フレーム制限
	if (mTickTime[0] < mTickTime[1] + mOneFrameTime) return false;

	// 経過時間算出
	mElapsedTime = (mTickTime[0] - mTickTime[1]) * mSecondsPerCount;
	if (mElapsedTime < 0.0f) mElapsedTime = 0.0f;

	// 次のフレームに備える
	mTickTime[1] = mTickTime[0];

	return true;
}

float HighResolutionTimer::GetElapsedTime()
{
	return static_cast<float>(mElapsedTime);
}
