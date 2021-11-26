#pragma once
#include <memory>

#include "CameraBase.h"

// 三人称視点のカメラ
class CameraTPS : public CameraBase
{
	// 定数
	static constexpr float LERP_FACTOR = 0.5f;	// 線形補完の係数
	static constexpr float DISTANCE = 15.0f;	// ターゲットからカメラの最大距離
	static constexpr float SPEED_X = 1.6f;		// 横方向の移動速度
	static constexpr float SPEED_Y = 1.0f;		// 縦方向の移動速度

public:// 関数

	// コンストラクタ
	CameraTPS();

	// デストラクタ
	~CameraTPS() = default;

	// 更新関数(オーバーライド)
	void Update(const Vector3& target) override;
};
