#pragma once
#include "CameraBase.h"

class CameraTitle : public CameraBase
{
	static constexpr float POS_Y = 2.5f; // カメラのY座標(適当

public:

	// 更新関数(オーバーライド)
	void Update(const Character* chara) override;
};