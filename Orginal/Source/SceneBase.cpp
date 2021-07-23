#include "SceneBase.h"

#include "lib/Framework.h"

#include "Define.h"

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
	float width = Define::SCREEN_WIDTH;
	float height = Define::SCREEN_HEIGHT;

	mGBufferColor.Initialize(width, height);
	mGBufferPosition.Initialize(width, height);
	mGBufferNormal.Initialize(width, height);

	// 定数バッファ作成
	mCBForDeferred.Create(sizeof(CBForDeferredPerFrame));

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
	context->OMSetRenderTargets(GBuffer::NUM, rtv, FRAMEWORK.GetDepthStencilView());
}

void SceneBase::DeactivateGBuffer()
{
	// ターゲットを戻す
	FRAMEWORK.SetRenderTarget();
}

void SceneBase::SetGBufferTexture(UINT startSlot)
{
	// GBufferをシェーダに渡す
	ID3D11ShaderResourceView* srv[GBuffer::NUM]
	{
		mGBufferColor.GetSRV(),
		mGBufferPosition.GetSRV(),
		mGBufferNormal.GetSRV()
	};
	FRAMEWORK.GetContext()->PSSetShaderResources(startSlot, GBuffer::NUM, srv);
}
