#pragma once

#include <windows.h>
#include <timeapi.h>

class HighResolutionTimer
{
	float mFrameRate = 60.0f;

	double mSecondsPerCount;
	double mDeltaTime; // 前のフレーム～現在のフレームまでの経過時間

	LONGLONG mFrame;	    // 1秒あたりのフレーム数
	LONGLONG mBaseTime;		// 基準の時間(タイマーは最初が毎回同じ数字じゃないから)
	LONGLONG mPausedTime;	// 計測が始まってから止まるまでの時間
	LONGLONG mStopTime;		// 停止した時の時間
	LONGLONG mLastTime;		// 1フレーム前の時間
	LONGLONG mThisTime;		// 現在の時間
	LONGLONG mTimeFreq;		// 周波数

	bool mStopped;

public:
	HighResolutionTimer();
	~HighResolutionTimer() = default;


	// Reset()が呼ばれてから経過した時間
	// Returns the total time elapsed since Reset() was called, NOT counting any
	// time when the clock is stopped.
	float TimeStamp() const;  // in seconds

	float TimeInterval() const;  // in seconds
	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop(); // Call when paused.
	bool Tick(); // Call every frame.
	void SetFrameRate(float frameRate);

};
