#pragma once
#include <memory>
#include <wrl.h>

#include "lib/ConstantBuffer.h"
#include "lib/RenderTarget.h"
#include "lib/Vector.h"

class Sprite;
class Shader;

/*
	ガウスブラーをかけるクラス
*/
class GaussianBlur
{
	// 定数
	static const int SAMPLING_COUNT = 10; // X, Yそれぞれのサンプリング数

private: // 変数

	// 定数バッファ関係
	struct CBuffer
	{
		float weight[SAMPLING_COUNT];
		Vector2 offset;
	};
	ConstantBuffer mConstBuffer;

	// ブラー関係
	std::unique_ptr<Shader> mHorizontalShader; // 横方向のブラーシェーダ
	RenderTarget mHorizontalBlur;			   // 横方向のブラーのレンダーテクスチャ
	std::unique_ptr<Shader> mVerticalShader;   // 縦方向のブラーシェーダ
	RenderTarget mVerticalBlur;				   // 縦方向のブラーのレンダーテクスチャ

	Vector2 mSize = {};			 // レンダーテクスチャのサイズ
	float mBlurStlength = 0.0f;	 // ブラーの強さ
	float mTexcelOffset = 1;	 // テクセルのオフセット
	bool mWeightUpdated = false; // ウェイトの更新済みフラグ

private: // 関数

	// ウェイトを更新する関数
	void UpdateWeights();

public:

	// コンストラクタ
	GaussianBlur();

	// デストラクタ
	~GaussianBlur();

	// 初期化関数
	void Initialize(const Vector2& targetSize = Vector2::ZERO, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT, float blurStlength = 100.0f, float offset = 1);
	
	// ブラーをかけたテクスチャを描画(要：Blur()
	void Render();
	
	// 引数のテクスチャにブラーをかける
	void Blur(const RenderTarget* orgSprite);

	// ブラーの強さを設定
	void SetBlurStlength(float stlength);

	RenderTarget* GetBlurTarget() { return &mVerticalBlur; };			// ブラー済みのレンダーテクスチャを取得
	void SetBlurTexture(UINT slot) { mVerticalBlur.SetTexture(slot); }	// ブラー済みのレンダーテクスチャをパイプラインに設定
};