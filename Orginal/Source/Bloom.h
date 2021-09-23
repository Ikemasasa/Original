#pragma once
#include <memory>

#include "Define.h"

class GaussianBlur;
class Shader;
class RenderTarget;

class Bloom
{
	// スクリーン / 4が最初のバッファサイズ
	static const int FIRST_BUFFER_SIZE_X = static_cast<int>(Define::SCREEN_WIDTH / 4);
	static const int FIRST_BUFFER_SIZE_Y = static_cast<int>(Define::SCREEN_HEIGHT / 4);
	static const int BUFFER_NUM = 4; // バッファの数
	static constexpr float BLUR_INTENSITY = 50.0f;

	std::unique_ptr<GaussianBlur> mTarget[BUFFER_NUM];
	std::unique_ptr<RenderTarget> mBrightness; // 輝度抽出テクスチャ
	std::unique_ptr<Shader> mExtractBrightness;

private:
	void MakeBuffer(RenderTarget* tex);
	void Compose(RenderTarget* tex);

public:
	Bloom();
	~Bloom();

	void Initialize();
	void Execute(RenderTarget* tex);
};