#pragma once
#include "CameraBase.h"

#include <memory>

#include "lib/Vector.h"

class Character;

class CameraBattle : public CameraBase
{
	float mTimer = 0;

	Vector3 mBeginPos;
	Vector3 mEndPos;
	float mLerpVelocity = 0.0f;
	float mLerpFactor = 0.0f;

public:
	CameraBattle();
	~CameraBattle() = default;
	void Update(const Character* target) override;
};