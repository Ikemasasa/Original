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
	// 輝度抽出シェーダ作成
	mExtractBrightness = std::make_unique<Shader>();
	mExtractBrightness->Load2D(L"Shaders/Bloom.fx", "VSMain", "PSMain");

	// 輝度抽出バッファ作成
	mBrightness = std::make_unique<RenderTarget>();
	mBrightness->Initialize(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);

	// MGF(マルチガウスフィルター)のバッファ作成
	int w = FIRST_BUFFER_SIZE_X;
	int h = FIRST_BUFFER_SIZE_Y;
	for (int i = 0; i < BUFFER_NUM; ++i)
	{
		mTarget[i] = std::make_unique<GaussianBlur>();

		Vector2 size(w, h);
		mTarget[i]->Initialize(size, DXGI_FORMAT_R8G8B8A8_UNORM, BLUR_INTENSITY);

		// w, hを半分にする
		w >>= 1;
		h >>= 1;
	}
}

void Bloom::MakeBuffer(RenderTarget* tex)
{
	// 輝度抽出
	mBrightness->Activate();
	tex->Render(mExtractBrightness.get());
	mBrightness->Deactivate();

	// 縮小バッファ作成
	const RenderTarget* render = mBrightness.get();
	for (int i = 0; i < BUFFER_NUM; ++i)
	{
		mTarget[i]->Blur(render);
		render = mTarget[i]->GetBlurTarget();
	}
}

void Bloom::Compose(RenderTarget* tex)
{
	// texと縮小バッファを合成

	// texをレンダーターゲットに(Activateするとクリアされるから直に
	ID3D11RenderTargetView* rtv = tex->GetRTV();
	ID3D11DepthStencilView* dsv = tex->GetDSV();
	FRAMEWORK.GetContext()->OMSetRenderTargets(1, &rtv, dsv);

	// 加算合成
	Blend::BLEND_MODE mode = Blend::GetMode();
	Blend::Set(Blend::ADD);

	for (int i = 0; i < BUFFER_NUM; ++i)
	{
		mTarget[i]->Render();
	}

	// ブレンドを戻す
	Blend::Set(mode);

	// tex->Deactivate();
}

void Bloom::Execute(RenderTarget* tex)
{
	// 今のレンダーターゲットを取得
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;
	FRAMEWORK.GetContext()->OMGetRenderTargets(1, &rtv, &dsv);

	// 縮小バッファ作成
	MakeBuffer(tex);

	// texと縮小バッファを合成
	Compose(tex);

	// レンダーターゲットを戻す
	FRAMEWORK.GetContext()->OMSetRenderTargets(1, &rtv, dsv);

	// 解放
	if (rtv)rtv->Release();
	if (dsv)dsv->Release();
}