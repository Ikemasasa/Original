#pragma once
#include "CameraBase.h"

class CameraEvent : public CameraBase
{
public: // 定数
	// カメラのモードEnum
	enum Mode
	{
		NONE,
		FIX,  // 固定カメラ
		LERP, // 線形補完カメラ
	};

private: // 変数
	Mode mMode = NONE; // モード保存
	Vector3 mTargetPos; // 移動先の座標
	float mLerpFactor;  // 線形補完の係数

public: // 関数
	void Update(const Vector3& target) override;

	void SetMode(Mode mode) { mMode = mode; }					// モードを設定
	void SetTargetPos(const Vector3& tp) { mTargetPos = mPos; } // 移動先の座標を設定
	void SetLerpFactor(float factor) { mLerpFactor = factor; }  // 線形補完の係数を設定
	void SetPos(const Vector3& pos) { mPos = pos; }
	void SetTarget(const Vector3& target) { mTarget = target; }
};