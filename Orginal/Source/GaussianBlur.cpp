#include "GaussianBlur.h"

#include "lib/Framework.h"
#include "lib/Shader.h"
#include "lib/Sprite.h"

GaussianBlur::GaussianBlur() = default;
GaussianBlur::~GaussianBlur() = default;

void GaussianBlur::Initialize(const Vector2& targetSize, DXGI_FORMAT format, float blurStlength, float offset)
{
	// ターゲット作成
	mSize = targetSize;
	mHorizontalBlur.Initialize(mSize.x, mSize.y, format);
	mVerticalBlur.Initialize(mSize.x, mSize.y, format);

	// シェーダ作成
	mHorizontalShader = std::make_unique<Shader>();
	mHorizontalShader->Load2D(L"Shaders/GaussianBlur.fx", "VSMain", "PSHorizontal");

	mVerticalShader = std::make_unique<Shader>();
	mVerticalShader->Load2D(L"Shaders/GaussianBlur.fx", "VSMain", "PSVertical");

	// 定数バッファ作成
	mConstBuffer.Create(sizeof(CBuffer));

	// ブラーの強さ
	mBlurStlength = blurStlength;

	// オフセット
	mTexcelOffset = offset;

	// ウェイトアップデートフラグ
	mWeightUpdated = false;

}

void GaussianBlur::Blur(const RenderTarget* orgSprite)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	if (!mWeightUpdated)
	{
		// ウェイト作成
		UpdateWeights();
	}

	// cbセット
	mConstBuffer.Set(0);

	// いまのレンダーターゲットを取得
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;
	context->OMGetRenderTargets(1, &rtv, &dsv);

	// 横方向
	mHorizontalBlur.Activate();
	orgSprite->Render(mHorizontalShader.get());
	mHorizontalBlur.Deactivate();

	// 縦方向
	mVerticalBlur.Activate();
	mHorizontalBlur.Render(mVerticalShader.get());
	mVerticalBlur.Deactivate();

	// レンダーターゲットを戻す
	context->OMSetRenderTargets(1, &rtv, dsv);
	
	// 解放
	if (rtv)rtv->Release();
	if (dsv)dsv->Release();
}

void GaussianBlur::Render(bool viewportOverwrite)
{
	if (viewportOverwrite)
	{
		// 両ブラー適用後のVerticalBlurを描画する
		// ビューポート設定

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


	// cb更新
	mConstBuffer.Update(&cb);
	mWeightUpdated = true;
}



