#pragma once

#include <windows.h>
#include <timeapi.h>

class HighResolutionTimer
{
	float mFrameRate = 60.0f;

	double mSecondsPerCount;
	double mDeltaTime; // �O�̃t���[���`���݂̃t���[���܂ł̌o�ߎ���

	LONGLONG mFrame;	    // 1�b������̃t���[����
	LONGLONG mBaseTime;		// ��̎���(�^�C�}�[�͍ŏ������񓯂���������Ȃ�����)
	LONGLONG mPausedTime;	// �v�����n�܂��Ă���~�܂�܂ł̎���
	LONGLONG mStopTime;		// ��~�������̎���
	LONGLONG mLastTime;		// 1�t���[���O�̎���
	LONGLONG mThisTime;		// ���݂̎���
	LONGLONG mTimeFreq;		// ���g��

	bool mStopped;

public:
	HighResolutionTimer();
	~HighResolutionTimer() = default;


	// Reset()���Ă΂�Ă���o�߂�������
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
