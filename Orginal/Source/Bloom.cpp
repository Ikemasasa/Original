#include "Bloom.h"

#include "lib/Blend.h"
#include "lib/Framework.h"
#include "lib/Shader.h"

#include "lib/RenderTarget.h"
#include "GaussianBlur.h"

Bloom::Bloom() = default;
Bloom::~Bloom() = default;

void Bloom::Initialize()
{
	// �P�x���o�V�F�[�_�쐬
	mExtractBrightness = std::make_unique<Shader>();
	mExtractBrightness->Load2D(L"Shaders/Bloom.fx", "VSMain", "PSMain");

	// �P�x���o�o�b�t�@�쐬
	mBrightness = std::make_unique<RenderTarget>();
	mBrightness->Initialize(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);

	// MGF(�}���`�K�E�X�t�B���^�[)�̃o�b�t�@�쐬
	int w = FIRST_BUFFER_SIZE_X;
	int h = FIRST_BUFFER_SIZE_Y;
	for (int i = 0; i < BUFFER_NUM; ++i)
	{
		mTarget[i] = std::make_unique<GaussianBlur>();

		Vector2 size(w, h);
		mTarget[i]->Initialize(size, DXGI_FORMAT_R8G8B8A8_UNORM, BLUR_INTENSITY);

		// w, h�𔼕��ɂ���
		w >>= 1;
		h >>= 1;
	}
}

void Bloom::MakeBuffer(RenderTarget* tex)
{
	// �P�x���o
	mBrightness->Activate();
	tex->Render(mExtractBrightness.get());
	mBrightness->Deactivate();

	// �k���o�b�t�@�쐬
	const RenderTarget* render = mBrightness.get();
	for (int i = 0; i < BUFFER_NUM; ++i)
	{
		mTarget[i]->Blur(render);
		render = mTarget[i]->GetBlurTarget();
	}
}

void Bloom::Compose(RenderTarget* tex)
{
	// tex�Ək���o�b�t�@������

	// tex�������_�[�^�[�Q�b�g��(Activate����ƃN���A����邩�璼��
	ID3D11RenderTargetView* rtv = tex->GetRTV();
	ID3D11DepthStencilView* dsv = tex->GetDSV();
	FRAMEWORK.GetContext()->OMSetRenderTargets(1, &rtv, dsv);

	// ���Z����
	Blend::BLEND_MODE mode = Blend::GetMode();
	Blend::Set(Blend::ADD);

	for (int i = 0; i < BUFFER_NUM; ++i)
	{
		mTarget[i]->Render();
	}

	// �u�����h��߂�
	Blend::Set(mode);

	// tex->Deactivate();
}

void Bloom::Execute(RenderTarget* tex)
{
	// ���̃����_�[�^�[�Q�b�g���擾
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;
	FRAMEWORK.GetContext()->OMGetRenderTargets(1, &rtv, &dsv);

	// �k���o�b�t�@�쐬
	MakeBuffer(tex);

	// tex�Ək���o�b�t�@������
	Compose(tex);

	// �����_�[�^�[�Q�b�g��߂�
	FRAMEWORK.GetContext()->OMSetRenderTargets(1, &rtv, dsv);

	// ���
	if (rtv)rtv->Release();
	if (dsv)dsv->Release();
}