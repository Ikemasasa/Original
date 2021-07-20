#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "lib/Vector.h"


class Light
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;

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
	void SetLightDir(const Vector4& lightDir) { mLightDir = lightDir; }
	void SetLightColor(const Vector4& lightColor) { mLightColor = lightColor; }
};