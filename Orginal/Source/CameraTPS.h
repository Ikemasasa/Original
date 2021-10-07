#pragma once
#include <memory>

#include "CameraBase.h"

class CameraTPS : public CameraBase
{
	static constexpr float LERP_FACTOR = 0.25f;

public:
	CameraTPS();
	~CameraTPS() = default;
	void Update(const Character* target) override;
};
