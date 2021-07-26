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
	// �V�F�[�_�쐬
	mGBufferShader = std::make_unique<Shader>();
	mGBufferShader->Load(L"Shaders/GBuffer.fx", "VSMain", "PSMain");
	
	// �����_�[�^�[�Q�b�g�쐬
	float width = Define::SCREEN_WIDTH;
	float height = Define::SCREEN_HEIGHT;

	mGBufferColor.Initialize(width, height);
	mGBufferPosition.Initialize(width, height);
	mGBufferNormal.Initialize(width, height);

	// �萔�o�b�t�@�쐬
	mCBForDeferred.Create(sizeof(CBForDeferredPerFrame));

	// �g���ł��낤 DirLight�V�F�[�_�쐬
	mDeferredDirLightShader = std::make_unique<Shader>();
	mDeferredDirLightShader->Load2D(L"Shaders/DeferredDirLight.fx", "VSMain", "PSMain");
}

void SceneBase::ActivateGBuffer(UINT startSlot)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// deactivate������ɂ܂�activate������Ƃ��A
	// ���͂Əo�͂ɂ��Ȃ�buffer�����邩�炻�̃G���[�΍�
	ID3D11ShaderResourceView* dummy[GBuffer::NUM] = {};
	context->PSSetShaderResources(startSlot, GBuffer::NUM, dummy);

	ID3D11RenderTargetView* rtv[GBuffer::NUM]
	{
		mGBufferColor.GetRTV(),
		mGBufferPosition.GetRTV(),
		mGBufferNormal.GetRTV()
	};
	
	// �����_�[�^�[�Q�b�g�N���A
	float clearColor[4] = { 0,0,0,0 };
	for (int i = 0; i < GBuffer::NUM; ++i)
	{
		context->ClearRenderTargetView(rtv[i], clearColor);
	}


	// �����_�[�^�[�Q�b�g�ݒ�
	context->OMSetRenderTargets(GBuffer::NUM, rtv, FRAMEWORK.GetDepthStencilView());
}

void SceneBase::DeactivateGBuffer()
{
	// �^�[�Q�b�g��߂�
	FRAMEWORK.SetRenderTarget();
}

void SceneBase::SetGBufferTexture(UINT startSlot)
{
	// GBuffer���V�F�[�_�ɓn��
	ID3D11ShaderResourceView* srv[GBuffer::NUM]
	{
		mGBufferColor.GetSRV(),
		mGBufferPosition.GetSRV(),
		mGBufferNormal.GetSRV()
	};
	FRAMEWORK.GetContext()->PSSetShaderResources(startSlot, GBuffer::NUM, srv);
}
