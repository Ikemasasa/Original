#pragma once
#include <memory>
#include <wrl.h>

#include "lib/ConstantBuffer.h"
#include "lib/RenderTarget.h"
#include "lib/Vector.h"

class Sprite;
class Shader;

class GaussianBlur
{
	static const int SAMPLING_COUNT = 10;

	struct CBuffer
	{
		float weight[SAMPLING_COUNT];
		Vector2 offset;
	};
	ConstantBuffer mConstBuffer;

	std::unique_ptr<Shader> mHorizontalShader;
	std::unique_ptr<Shader> mVerticalShader;
	RenderTarget mHorizontalBlur;
	RenderTarget mVerticalBlur;

	Vector2 mSize = {};
	float mBlurStlength = 0.0f;
	float mTexcelOffset = 1;
	bool mWeightUpdated = false;

private:
	void UpdateWeights();

public:
	GaussianBlur();
	~GaussianBlur();

	void Initialize(const Vector2& targetSize = Vector2::ZERO, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT, float blurStlength = 100.0f, float offset = 1);
	void Blur(const RenderTarget* orgSprite);
	void Render();

	void SetBlurStlength(float stlength);

	RenderTarget* GetBlurTarget() { return &mVerticalBlur; };
};