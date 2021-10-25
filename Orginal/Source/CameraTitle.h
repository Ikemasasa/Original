#pragma once
#include "CameraBase.h"

class CameraTitle : public CameraBase
{
	static constexpr float POS_Y = 2.5f;
public:
	void Update(const Character* chara) override;
};