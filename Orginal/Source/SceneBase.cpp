#include "SceneBase.h"

#include "lib/Framework.h"

// staticmenba
Light SceneBase::mLight;

void SceneBase::CreatePostEffectShader()
{
	mPostEffect = std::make_unique<Shader>();
	mPostEffect->Load2D(L"Shaders/PostEffect.fx", "VSMain", "PSMain");
}

void SceneBase::InitializeGBuffer()
{
	// シェーダ作成
	mGBufferShader = std::make_unique<Shader>();
	mGBufferShader->Load(L"Shaders/GBuffer.fx", "VSMain", "PSMain");
	
	// レンダーターゲット作成
	mGBufferColor.Initialize();
	mGBufferPosition.Initialize();
	mGBufferNormal.Initialize();


	// 使うであろう DirLightシェーダ作成
	mDeferredDirLightShader = std::make_unique<Shader>();
	mDeferredDirLightShader->Load2D(L"Shaders/DeferredDirLight.fx", "VSMain", "PSMain");
}

void SceneBase::ActivateGBuffer(UINT startSlot)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// deactivateした後にまたactivateをするとき、
	// 入力と出力におなじbufferが入るからそのエラー対策
	ID3D11ShaderResourceView* dummy[GBuffer::NUM] = {};
	context->PSSetShaderResources(startSlot, GBuffer::NUM, dummy);

	ID3D11RenderTargetView* rtv[GBuffer::NUM]
	{
		mGBufferColor.GetRTV(),
		mGBufferPosition.GetRTV(),
		mGBufferNormal.GetRTV()
	};
	
	// レンダーターゲットクリア
	float clearColor[4] = { 0,0,0,0 };
	for (int i = 0; i < GBuffer::NUM; ++i)
	{
		context->ClearRenderTargetView(rtv[i], clearColor);
	}


	// レンダーターゲット設定
	context->OMSetRenderTargets(GBuffer::NUM, rtv, nullptr);
}

void SceneBase::DeactivateGBuffer(UINT startSlot)
{
	// ターゲットを戻す
	FRAMEWORK.SetRenderTarget();

	// GBufferをシェーダに渡す
	ID3D11ShaderResourceView* srv[GBuffer::NUM]
	{
		mGBufferColor.GetSRV(),
		mGBufferPosition.GetSRV(),
		mGBufferNormal.GetSRV()
	};
	FRAMEWORK.GetContext()->PSSetShaderResources(startSlot, GBuffer::NUM, srv);
}
