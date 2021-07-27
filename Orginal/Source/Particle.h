#pragma once

#include "lib/Vector.h"

class Particle
{
	struct Vertex
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 tex;
		Vector4 color;
		Vector4 param; // x:回転 yzw:アニメ
	};

	float mLifeTime; // 生存時間
	float mDuration; // エミッターの有効時間
	float RateOverTime; // 個 / 秒(1秒間に何個出すか)
	float EmitCount;

	Vector3 EmitCenter; // 発生中心
	float mEmitRadius;  // 発生半径
};