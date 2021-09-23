#include "GBuffer.h"

#include "lib/Framework.h"
#include "lib/Shader.h"

void GBuffer::Initialize(float width, float height)
{
	// �V�F�[�_�쐬
	mShader = std::make_unique<Shader>();
	mShader->Load(L"Shaders/GBuffer.fx", "VSMain", "PSMain");

	// �����_�[�^�[�Q�b�g�쐬
	mTargets[COLOR].Initialize(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
	mTargets[POS].Initialize(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	mTargets[NORMAL].Initialize(width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);

	// �^�[�Q�b�g�T�C�Y�ۑ�
	mTargetSize.x = width;
	mTargetSize.y = height;

	mSlot = 0;
}

void GBuffer::Activate()
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// deactivate������ɂ܂�activate������Ƃ��A
	// ���͂Əo�͂ɂ��Ȃ�buffer�����邩�炻�̃G���[�΍�
	if (mSlot != -1)
	{
		ID3D11ShaderResourceView* dummy[GBuffer::NUM] = {};
		context->PSSetShaderResources(mSlot, GBuffer::NUM, dummy);
		mSlot = -1;
	}


	// �����_�[�^�[�Q�b�g�擾
	ID3D11RenderTargetView* rtv[GBuffer::NUM];
	for (int i = 0; i < NUM; ++i) rtv[i] = mTargets[i].GetRTV();

	// �����_�[�^�[�Q�b�g�N���A
	float clearColor[4] = { 0,0,0,1 };
	for (int i = 0; i < GBuffer::NUM; ++i) context->ClearRenderTargetView(rtv[i], clearColor);

	// �����_�[�^�[�Q�b�g�ݒ�
	context->OMSetRenderTargets(GBuffer::NUM, rtv, FRAMEWORK.GetDepthStencilView());

}

void GBuffer::Deactivate()
{
	// �^�[�Q�b�g��߂�
	FRAMEWORK.SetRenderTarget();
}

void GBuffer::SetTexture(UINT startSlot)
{
	// GBuffer���V�F�[�_�ɓn��
	// �V�F�[�_�[���\�[�X�擾
	ID3D11ShaderResourceView* srv[GBuffer::NUM];
	for (int i = 0; i < NUM; ++i) srv[i] = mTargets[i].GetSRV();

	// �V�F�[�_�[���\�[�X�ݒ�
	mSlot = startSlot;
	FRAMEWORK.GetContext()->PSSetShaderResources(mSlot, GBuffer::NUM, srv);
}
