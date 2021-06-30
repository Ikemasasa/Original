#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>

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
	static const int SHADOWMAP_SRV_SLOT = 14;

	RenderTarget mShadowMap;
	RenderTarget mVarianceShadowMap;
	std::unique_ptr<Shader> mShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer;

	Vector3 mTarget;

public:
	ShadowMap();
	~ShadowMap();

	void Initialize();
	void Activate(const DirectX::XMFLOAT4& lightDir);
	void Deactivate();
	Shader* GetShader()const { return mShader.get(); }

};

