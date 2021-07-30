#pragma once
#include <memory>
#include <wrl.h>

#include "lib/RenderTarget.h"
#include "lib/Vector.h"

class Sprite;
class Shader;

class GaussianBlur
{
	static const int SAMPLING_COUNT = 10;
	static const int OFFSET = 1;

	struct CBuffer
	{
		float weight[SAMPLING_COUNT];
		Vector2 offset;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer;

	std::unique_ptr<Shader> mHorizontalShader;
	std::unique_ptr<Shader> mVerticalShader;
	RenderTarget mHorizontalBlur;
	RenderTarget mVerticalBlur;

	Vector2 mSize;
	float mBlurStlength = 0.0f;

private:
	void UpdateWeights();

public:
	GaussianBlur() = default;
	~GaussianBlur() = default;

	void Initialize(const Vector2& targetSize = Vector2::ZERO, float blurStlength = 100.0f);
	void Blur(const RenderTarget* orgSprite);
	void Blur(const Sprite* sprite, const Vector2& pos, const Vector2& scale);

	void SetBlurStlength(float stlength);
};