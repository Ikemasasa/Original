#pragma once
#include <memory>

#include "CameraBase.h"

class Character;

class CameraDebug : public CameraBase
{
	POINT mOldMp = {};

public:
	CameraDebug() : CameraBase() { ShowCursor(false); }

	void Update(const Character* target) override;

};