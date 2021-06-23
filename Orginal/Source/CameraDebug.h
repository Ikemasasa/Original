#pragma once
#include <memory>

#include "CameraBase.h"

class Actor;

class CameraDebug : public CameraBase
{
	POINT mOldMp = {};

public:
	CameraDebug() : CameraBase() { ShowCursor(false); }

	void Update(const Actor* target) override;

};