#pragma once
#include <memory>

#include "lib/RenderTarget.h"

class Shader;

/*
	GBufferクラス
	ディファードレンダリング用
	今はカラー、座標、法線 が保存できる
*/
class GBuffer
{
public: // 変数
	enum Kind { COLOR, POS, NORMAL, NUM };

private:
	std::unique_ptr<Shader> mShader; // MRTのシェ－ダ
	RenderTarget mTargets[NUM];		 // GBufferのレンダーテクスチャ(ここにMRTで描画する)
	Vector2 mTargetSize;			 // GBufferのサイズ
	UINT mSlot = 0;					 // パイプラインに設定したスロット

public:// 関数

	// コンストラクタ
	GBuffer() = default;

	// デストラクタ
	virtual ~GBuffer() = default;

	// 初期化関数
	void Initialize(float width, float height);
	
	// MRTの設定
	void Activate();

	// ターゲットを戻す
	void Deactivate();

	// パイプラインにテクスチャを設定
	void SetTexture(UINT startSlot);
	
	RenderTarget* GetColorTarget() { return &mTargets[COLOR]; } // カラーのレンダーテクスチャを取得
	Vector2 GetTargetSize() const { return mTargetSize; }       // GBufferのサイズをしゅとく
};