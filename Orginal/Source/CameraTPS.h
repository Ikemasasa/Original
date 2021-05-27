#pragma once
#include <memory>

#include "CameraBase.h"

class Actor;

class CameraTPS : public CameraBase
{
public:
	CameraTPS();
	~CameraTPS() = default;
	void Update(const std::shared_ptr<Actor>& target) override;
};
