#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "lib/ConstantBuffer.h"
#include "lib/Vector.h"

class Light
{
	// ライト最大数
	static const int DIR_MAX = 4;
	static const int POINT_MAX = 16;

public:
	struct DirLight
	{
		Vector4 dir;
		Vector4 color;
	};

	struct PointLight
	{
		Vector4 pos; // xyz:座標, w:range
		Vector4 color;
	};

private:
	ConstantBuffer mCB;

	Vector4 mLightDir;
	Vector4 mLightColor;

	struct CBForLight
	{
		Vector4 lightDir;
		Vector4 lightColor;
	};

public:
	void CreateConstBuffer();
	void SetConstBuffer(int cbSlot);
	void UpdateConstBuffer();

	// ゲッター
	Vector4 GetLightDir() { return mLightDir; }
	Vector4 GetLightColor() { return mLightColor; }

	// セッター
	void SetLightDir(const Vector4& lightDir);
	void SetLightColor(const Vector4& lightColor) { mLightColor = lightColor; }
};