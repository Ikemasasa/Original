#include "DeferredRenderer.h"

#include "lib/Blend.h"
#include "lib/ConstantBuffer.h"
#include "lib/Framework.h"
#include "lib/Renderer2D.h"
#include "lib/Shader.h"

#include "Define.h"

void DeferredRenderer::Initialize(const Vector2 gbufferSize)
{
	// GBuffer作成
	mGBuffer = std::make_unique<GBuffer>();
	mGBuffer->Initialize(gbufferSize.x, gbufferSize.y);

	// シェーダ作成
	mGBufferShader = std::make_unique<Shader>();
	mGBufferShader->Load(L"Shaders/GBuffer.fx", "VSMain", "PSMain");

	mDirLightShader = std::make_unique<Shader>();
	mDirLightShader->Load2D(L"Shaders/DeferredDirLight.fx", "VSMain", "PSMain");

	mPointLightShader = std::make_unique<Shader>();
	mPointLightShader->Load2D(L"Shaders/DeferredPointLight.fx", "VSMain", "PSMain");

	mSpotLightShader = std::make_unique<Shader>();
	mSpotLightShader->Load2D(L"Shaders/DeferredSpotLight.fx", "VSMain", "PSMain");

	// 定数バッファ作成
	mCBPerFrame = std::make_unique<ConstantBuffer>();
	mCBPerFrame->Create(sizeof(DeferredPerFrame));

	mCBDirLight = std::make_unique<ConstantBuffer>();
	mCBDirLight->Create(sizeof(DirLight));

	mCBPointLight = std::make_unique<ConstantBuffer>();
	mCBPointLight->Create(sizeof(CBForPointLight));

	mCBSpotLight = std::make_unique<ConstantBuffer>();
	mCBSpotLight->Create(sizeof(CBForSpotLight));
}

void DeferredRenderer::ActivateGBuffer()
{
	mGBuffer->Activate();
}

void DeferredRenderer::DeactivateGBuffer()
{
	mGBuffer->Deactivate();
}

void DeferredRenderer::SetGBufferTexture(UINT startSlot)
{
	mGBuffer->SetTexture(startSlot);
}

void DeferredRenderer::SetCBPerFrame(const Vector3& eyePos)
{
	DeferredPerFrame cb;
	cb.eyePos = Vector4(eyePos, 1.0f);
	mCBPerFrame->Update(&cb);
}

void DeferredRenderer::SetDirLight(std::vector<DirLight>& dirLights)
{
	mDirLights = dirLights;
}

void DeferredRenderer::SetPointLight(std::vector<PointLight>& pointLights)
{
	mPointLights = pointLights;
}

void DeferredRenderer::SetSpotLight(std::vector<SpotLightArgu>& spotLights)
{
	for (int i = 0; i < SPOT_LIGHT_MAX; ++i)
	{
		SpotLight sl = {};
		if (i < spotLights.size())
		{
			auto& light = spotLights[i];
			// dir計算
			Vector3 dir = light.target - light.pos;
			
			// inner, outer算出
			float toRad = Define::PI / 180.0f;
			light.inner = cosf(light.inner * toRad);
			light.outer = cosf(light.outer * toRad);

			// dir正規化、range算出
			float d = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
			float range = d * 3.0f;
			if (d > 0) dir /= d; 
			sl.pos = Vector4(spotLights[i].pos, range);
			sl.dir = Vector4(dir, light.inner);
			sl.color = Vector4(light.rgb, light.outer);
		}
		else
		{
			sl.pos = Vector4(0.0f, 0.0f, 0.0f, 0.001f);
			sl.dir = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			sl.color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		mSpotLights.emplace_back(sl);
	}
}

void DeferredRenderer::ClearLight()
{
	mDirLights.clear();
	mPointLights.clear();
	mSpotLights.clear();
}

void DeferredRenderer::Render()
{
	// 加算合成
	Blend::Set(Blend::ADD);

	// 描画用変数
	const Vector2 POS(Vector2::ZERO);
	const Vector2 SCALE(Vector2::ONE);
	const Vector2 TEXPOS(Vector2::ZERO);
	const Vector2 SIZE(mGBuffer->GetTargetSize());

	mCBPerFrame->Set(0);

	// dirlight
	for (auto& dirLight : mDirLights)
	{
		DirLight cb;
		cb.dir = dirLight.dir;
		cb.color = dirLight.color;
		mCBDirLight->Update(&cb);
		mCBDirLight->Set(CB_LIGHT_SLOT);

		Renderer2D::GetInstance().Render(mDirLightShader.get(), POS, SCALE, TEXPOS, SIZE);
	}

	// pointlight
	if (mPointLights.size() > 0)
	{
		CBForPointLight cb;
		for (int i = 0; i < POINT_LIGHT_MAX; ++i)
		{
			if (i < mPointLights.size())
			{
				cb.pointLight[i].pos = mPointLights[i].pos;
				cb.pointLight[i].color = mPointLights[i].color;
			}
			else
			{
				cb.pointLight[i].pos = Vector4(0, 0, 0, 0.001f);
				cb.pointLight[i].color = Vector4(0, 0, 0, 1);
			}
		}

		mCBPointLight->Update(&cb);
		mCBPointLight->Set(CB_LIGHT_SLOT);
		Renderer2D::GetInstance().Render(mPointLightShader.get(), POS, SCALE, TEXPOS, SIZE);
	}

	// spotlight
	if (mSpotLights.size() > 0)
	{
		CBForSpotLight cb;
		for (int i = 0; i < SPOT_LIGHT_MAX; ++i)
		{
			if (i < mSpotLights.size())
			{
				cb.spotLight[i].pos = mSpotLights[i].pos;
				cb.spotLight[i].dir = mSpotLights[i].dir;
				cb.spotLight[i].color = mSpotLights[i].color;
			}
			else
			{
				cb.spotLight[i].pos = Vector4(0.0f, 0.0f, 0.0f, 0.001f);
				cb.spotLight[i].dir = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
				cb.spotLight[i].color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		mCBSpotLight->Update(&cb);
		mCBSpotLight->Set(CB_LIGHT_SLOT);
		Renderer2D::GetInstance().Render(mSpotLightShader.get(), POS, SCALE, TEXPOS, SIZE);

	}

	// アルファ合成に戻す
	Blend::Set(Blend::ALPHA);
}