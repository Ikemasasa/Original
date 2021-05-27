#pragma once
#include "CameraBase.h"

#include <memory>

#include "Actor.h"

class CameraBattle : public CameraBase
{
public:
	CameraBattle();
	~CameraBattle() = default;
	void Update(const std::shared_ptr<Actor>& target) override;
};