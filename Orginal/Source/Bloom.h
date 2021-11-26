#pragma once
#include <memory>

#include "Define.h"

class GaussianBlur;
class Shader;
class RenderTarget;

// RenderTargetからブルームを作るクラス
class Bloom
{
	// 定数
	// スクリーン / 4が最初のバッファサイズ
	static const int FIRST_BUFFER_SIZE_X = static_cast<int>(Define::SCREEN_WIDTH / 4);
	static const int FIRST_BUFFER_SIZE_Y = static_cast<int>(Define::SCREEN_HEIGHT / 4);
	static const int BUFFER_NUM = 4;			   // バッファの数
	static constexpr float BLUR_INTENSITY = 50.0f; // ブラーの強さ

private: // 変数
	std::unique_ptr<GaussianBlur> mTarget[BUFFER_NUM]; // ブラーをかけたテクスチャ
	std::unique_ptr<RenderTarget> mBrightness;		   // 輝度抽出テクスチャ
	std::unique_ptr<Shader> mExtractBrightness;		   // 輝度抽出シェーダ

private: // 関数

	// 引数から輝度抽出して、ブラーをかけたテクスチャを作る
	void MakeBuffer(RenderTarget* tex);

	// ブラーをかけたテクスチャを合成する
	void Compose(RenderTarget* tex);

public:
	// コンストラクタ
	Bloom();

	// デストラクタ
	~Bloom();

	// 初期化関数
	void Initialize();

	// ブルーム実行関数(引数のテクスチャから作る)
	void Execute(RenderTarget* tex);
};