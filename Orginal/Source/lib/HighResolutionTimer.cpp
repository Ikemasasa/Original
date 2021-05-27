#include "HighResolutionTimer.h"

HighResolutionTimer::HighResolutionTimer() : mFrameRate(60.0f), mSecondsPerCount(0.0), mDeltaTime(-1.0), mStopTime(0), mPausedTime(0), mStopped(false)
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&mTimeFreq));
	mSecondsPerCount = 1.0 / static_cast<double>(mTimeFreq);
	mFrame = (LONGLONG)(mTimeFreq / mFrameRate);

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mThisTime));
	mBaseTime = mThisTime;
	mLastTime = mThisTime;
}

float HighResolutionTimer::TimeStamp() const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// stop_time - base_time includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused_time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  base_time       stop_time        start_time     stop_time    this_time

	if (mStopped)
	{
		return static_cast<float>(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}

	// The distance this_time - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from this_time:  
	//
	//  (this_time - paused_time) - base_time 
	//
	//                     |<--paused_time-->|
	// ----*---------------*-----------------*------------*------> time
	//  base_time       stop_time        start_time     this_time
	else
	{
		return static_cast<float>(((mThisTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}

float HighResolutionTimer::TimeInterval() const
{
	return static_cast<float>(mDeltaTime);
}

void HighResolutionTimer::Reset()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mThisTime));
	mBaseTime = mThisTime;
	mLastTime = mThisTime;

	mStopTime = 0;
	mStopped = false;
}

void HighResolutionTimer::Start()
{
	LONGLONG startTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  base_time       stop_time        start_time     
	if (mStopped)
	{
		mPausedTime += (startTime - mStopTime);
		mLastTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void HighResolutionTimer::Stop()
{
	if (!mStopped)
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mStopTime));
		mStopped = true;
	}
}

bool HighResolutionTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return true;
	}

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mThisTime));

	// ÉtÉåÅ[ÉÄêßå¿
	if (mThisTime < mLastTime + mFrame) return false;

	// Time difference between this frame and the previous.
	mDeltaTime = (mThisTime - mLastTime) * mSecondsPerCount;

	// åoâﬂéûä‘
	LONGLONG dTime = mThisTime - mLastTime;

	// Prepare for next frame.
	mLastTime = mThisTime;

	if (dTime > 40000) mLastTime = mThisTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}

	return true;
}

void HighResolutionTimer::SetFrameRate(float frameRate)
{
	mFrameRate = frameRate;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&mTimeFreq));
	mSecondsPerCount = 1.0 / static_cast<double>(mTimeFreq);
	mFrame = (LONGLONG)(mTimeFreq / mFrameRate);
}
