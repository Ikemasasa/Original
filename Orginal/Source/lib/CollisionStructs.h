#pragma once
#include "Vector.h"

struct SPHERE
{
	Vector3 center;
	float radius;
	void Update(const Vector3& center, const float radius)
	{
		this->center = center;
		this->radius = radius;
	}
};

struct AABB
{
	Vector3 min = { FLT_MIN, FLT_MIN, FLT_MIN };
	Vector3 max = { FLT_MAX, FLT_MAX, FLT_MAX };
};

struct OBB
{
	Vector3 center;
	Vector3 norDirection[3];   // 方向ベクトル
	Vector3 length;         // 各軸方向の長さ
};

struct RAY
{
	Vector3 point;		// 始点
	Vector3 direction;	// 方向
	float length;		// 長さ

	Vector3 GetRay() const { return direction * length; }
};

struct CAPSULE
{
	Vector3 centerTop;
	Vector3 centerBottom;
	float radius;

	Vector3 GetDistVector() const { return centerBottom - centerTop; }
};