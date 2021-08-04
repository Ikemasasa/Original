#pragma once
#include "CameraBase.h"

#include <memory>

class Character;

class CameraBattle : public CameraBase
{
public:
	CameraBattle();
	~CameraBattle() = default;
	void Update(const Character* target) override;
};