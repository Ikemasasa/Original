#pragma once
#include <DirectXMath.h>
#include <memory>

#include "GaussianBlur.h"

#include "lib/ConstantBuffer.h"
#include "lib/RenderTarget.h"
#include "lib/Shader.h"
#include "lib/Vector.h"

class ShadowMap
{
	struct CBShadow
	{
		DirectX::XMFLOAT4X4 shadowVP;
	};

	static constexpr float SHADOWMAP_X = 4098;
	static constexpr float SHADOWMAP_Y = 4098;

	static constexpr float BLUR_STRENGTH = 20;

	RenderTarget mShadowMap;
	RenderTarget mVarianceShadowMap;
	GaussianBlur mGaussianBlur;
	std::unique_ptr<Shader> mShader;
	ConstantBuffer mConstBuffer;

	Vector3 mTarget;

public:
	ShadowMap();
	~ShadowMap();

	void Initialize();
	void Activate(const Vector4& lightDir, const Vector3& lightPos);
	void Deactivate();
	void SetTexture(UINT textureSlot);
	Shader* GetShader()const { return mShader.get(); }
};

