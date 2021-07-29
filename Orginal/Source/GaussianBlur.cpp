#include "GaussianBlur.h"

#include "lib/Framework.h"
#include "lib/Shader.h"
#include "lib/Sprite.h"

void GaussianBlur::Initialize(const Vector2& targetSize, float blurStlength)
{
	// �V�F�[�_
	mHorizontalShader = std::make_unique<Shader>();
	mHorizontalShader->Load2D(L"Shaders/GaussianBlur.fx", "VSMain", "PSHorizontal");
	
	mVerticalShader = std::make_unique<Shader>();
	mVerticalShader->Load2D(L"Shaders/GaussianBlur.fx", "VSMain", "PSVertical");

	// �^�[�Q�b�g�쐬
	mSize = targetSize;
	mHorizontalBlur.Initialize(mSize.x, mSize.y);
	mVerticalBlur.Initialize(mSize.x, mSize.y);

	// �萔�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(CBuffer);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = FRAMEWORK.GetDevice()->CreateBuffer(&bd, NULL, mConstBuffer.GetAddressOf());
	if (FAILED(hr)) return;

	// �u���[�̋���
	mBlurStlength = blurStlength;

	// �E�F�C�g�쐬
	UpdateWeights();
}

void GaussianBlur::Blur(const RenderTarget* orgSprite)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// cb�Z�b�g
	context->PSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

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

	// �u���[���������摜��`��
	// �����_�[�^�[�Q�b�g
	context->OMSetRenderTargets(1, &rtv, dsv);
	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT viewport;
	viewport.Width = mSize.x;
	viewport.Height = mSize.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);

	mVerticalBlur.Render(nullptr);
	
	// ���
	if (rtv)rtv->Release();
	if (dsv)dsv->Release();
}

void GaussianBlur::SetBlurStlength(float stlength)
{
	mBlurStlength = stlength;
	UpdateWeights();
}

void GaussianBlur::UpdateWeights()
{
	float total = 0;
	float d = mBlurStlength * mBlurStlength * 0.01f;

	CBuffer cb = {};
	cb.offset.x = OFFSET / mSize.x;
	cb.offset.y = OFFSET / mSize.y;
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
	FRAMEWORK.GetContext()->UpdateSubresource(mConstBuffer.Get(), 0, nullptr, &cb, 0, 0);
}

