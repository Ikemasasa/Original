#include "SceneBase.h"

#include "lib/Framework.h"

#include "Define.h"

// staticmenba
Light SceneBase::mLight;

void SceneBase::InitializeBaseAll()
{
	//mRamp->Set(Define::RAMP_TEXTURE_SLOT);
	mPostEffect->Load2D(L"Shaders/PostEffect.fx", "VSMain", "PSMain");
	mShadowMap->Initialize();
	mSceneTarget->Initialize(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT);
	mPostEffectTarget->Initialize(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT);
	mBloom->Initialize();
	mDeferredRenderer->Initialize(Vector2(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT));
}

void SceneBase::CreateBaseAll()
{
	//mRamp = std::make_unique<Sprite>(L"Data/Image/Ramp.png");
	mPostEffect = std::make_unique<Shader>();
	mShadowMap = std::make_unique<ShadowMap>();
	mSceneTarget = std::make_unique<RenderTarget>();
	mPostEffectTarget = std::make_unique<RenderTarget>();
	mBloom = std::make_unique<Bloom>();
	mDeferredRenderer = std::make_unique<DeferredRenderer>();
}

void SceneBase::ReleaseBaseAll()
{
	//mRamp->UnLoad();
	mPostEffect->UnLoad();
}
