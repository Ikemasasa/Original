#include "GaussianBlur.h"

#include "lib/Framework.h"
#include "lib/Shader.h"
#include "lib/Sprite.h"

GaussianBlur::GaussianBlur() = default;
GaussianBlur::~GaussianBlur() = default;

void GaussianBlur::Initialize(const Vector2& targetSize, DXGI_FORMAT format, float blurStlength, float offset)
{
	// �^�[�Q�b�g�쐬
	mSize = targetSize;
	mHorizontalBlur.Initialize(mSize.x, mSize.y, format);
	mVerticalBlur.Initialize(mSize.x, mSize.y, format);

	// �V�F�[�_�쐬
	mHorizontalShader = std::make_unique<Shader>();
	mHorizontalShader->Load2D(L"Shaders/GaussianBlur.fx", "VSMain", "PSHorizontal");

	mVerticalShader = std::make_unique<Shader>();
	mVerticalShader->Load2D(L"Shaders/GaussianBlur.fx", "VSMain", "PSVertical");

	// �萔�o�b�t�@�쐬
	mConstBuffer.Create(sizeof(CBuffer));

	// �u���[�̋���
	mBlurStlength = blurStlength;

	// �I�t�Z�b�g
	mTexcelOffset = offset;

	// �E�F�C�g�A�b�v�f�[�g�t���O
	mWeightUpdated = false;

}

void GaussianBlur::Blur(const RenderTarget* orgSprite)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	if (!mWeightUpdated)
	{
		// �E�F�C�g�쐬
		UpdateWeights();
	}

	// cb�Z�b�g
	mConstBuffer.Set(0);

	// ���܂̃����_�[�^�[�Q�b�g���擾
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;
	context->OMGetRenderTargets(1, &rtv, &dsv);

	// ������
	mHorizontalBlur.Activate();
	orgSprite->Render(mHorizontalShader.get());
	mHorizontalBlur.Deactivate();

	// �c����
	mVerticalBlur.Activate();
	mHorizontalBlur.Render(mVerticalShader.get());
	mVerticalBlur.Deactivate();

	// �����_�[�^�[�Q�b�g��߂�
	context->OMSetRenderTargets(1, &rtv, dsv);
	
	// ���
	if (rtv)rtv->Release();
	if (dsv)dsv->Release();
}

void GaussianBlur::Render(bool viewportOverwrite)
{
	if (viewportOverwrite)
	{
		// ���u���[�K�p���VerticalBlur��`�悷��
		// �r���[�|�[�g�ݒ�

		//D3D11_VIEWPORT viewport;
		//viewport.Width = mSize.x;
		//viewport.Height = mSize.y;
		//viewport.MinDepth = 0.0f;
		//viewport.MaxDepth = 1.0f;
		//viewport.TopLeftX = 0;
		//viewport.TopLeftY = 0;
		//FRAMEWORK.GetContext()->RSSetViewports(1, &viewport);
	}

	mVerticalBlur.Render(nullptr);
}

void GaussianBlur::SetBlurStlength(float stlength)
{
	mBlurStlength = stlength;
	mWeightUpdated = false;
}

void GaussianBlur::UpdateWeights()
{
	float total = 0;
	float d = mBlurStlength * mBlurStlength * 0.01f;

	CBuffer cb = {};
	cb.offset.x = mTexcelOffset / mSize.x;
	cb.offset.y = mTexcelOffset / mSize.y;
	for (int i = 0; i < SAMPLING_COUNT; i++)
	{
		// Offset position
		float r = 1.0f + 2.0f * i;
		cb.weight[i] = expf(-0.5f * (r * r) / d);

		total += cb.weight[i];
	}
	
	for (int i = 0; i < SAMPLING_COUNT; i++)
	{
		cb.weight[i] /= total;
	}


	// cb�X�V
	mConstBuffer.Update(&cb);
	mWeightUpdated = true;
}



