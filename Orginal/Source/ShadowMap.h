#pragma once
#include <memory>

#include "GaussianBlur.h"

#include "lib/ConstantBuffer.h"
#include "lib/RenderTarget.h"
#include "lib/Shader.h"
#include "lib/Vector.h"
#include "lib/Matrix.h"

// シャドウマップを生成、設定するクラス
class ShadowMap
{
	// 定数
	static constexpr float SHADOWMAP_X = 4098; // シャドウマップのテクスチャサイズ
	static constexpr float SHADOWMAP_Y = 4098; // シャドウマップのテクスチャサイズ
	static constexpr float BLUR_STRENGTH = 20; // VSMのブラーの強さ

private: // 変数
	RenderTarget mShadowMap;		 // シャドウマップのレンダーテクスチャ
	RenderTarget mVarianceShadowMap; // VSMのレンダーテクスチャ
	GaussianBlur mGaussianBlur;		 // ガウスブラーをかけるクラス
	std::unique_ptr<Shader> mShader; // シャドウマップシェーダー

	struct CBShadow
	{
		Matrix shadowVP;
	};
	ConstantBuffer mConstBuffer;	 // 定数バッファ

	Matrix mShadowProj;	// シャドウマップの用のプロジェクション行列
	Vector3 mTarget;	// シャドウマップ用のカメラのターゲット

public:
	ShadowMap();
	~ShadowMap();

	void Initialize();
	void Activate(const Vector3& target, const Vector3& dir, float maxDistance =  170.0f);
	void Deactivate();
	void SetTexture(UINT textureSlot);
	Shader* GetShader()const { return mShader.get(); }
};

