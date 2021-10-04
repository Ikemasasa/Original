#include "HighResolutionTimer.h"

void HighResolutionTimer::Initialize()
{
	mElapsedTime = 0.0f;
	mPausedTime = 0;
	mIsStopped = false;
	
	// ���g��
	LONGLONG countsPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
	mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
	mOneFrameTime = (LONGLONG)(countsPerSec / 60);

	// ���O����
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

	// ���݂̎��Ԏ擾
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mTickTime[0]));

	// �t���[������
	if (mTickTime[0] < mTickTime[1] + mOneFrameTime) return false;

	// �o�ߎ��ԎZ�o
	mElapsedTime = (mTickTime[0] - mTickTime[1]) * mSecondsPerCount;
	if (mElapsedTime < 0.0f) mElapsedTime = 0.0f;

	// ���̃t���[���ɔ�����
	mTickTime[1] = mTickTime[0];

	return true;
}

float HighResolutionTimer::GetElapsedTime()
{
	return static_cast<float>(mElapsedTime);
}
