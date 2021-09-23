#include "GBuffer.h"

#include "lib/Framework.h"
#include "lib/Shader.h"

void GBuffer::Initialize(float width, float height)
{
	// シェーダ作成
	mShader = std::make_unique<Shader>();
	mShader->Load(L"Shaders/GBuffer.fx", "VSMain", "PSMain");

	// レンダーターゲット作成
	mTargets[COLOR].Initialize(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
	mTargets[POS].Initialize(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	mTargets[NORMAL].Initialize(width, height, DXGI_FORMAT_R16G16B16A16_FLOAT);

	// ターゲットサイズ保存
	mTargetSize.x = width;
	mTargetSize.y = height;

	mSlot = 0;
}

void GBuffer::Activate()
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// deactivateした後にまたactivateをするとき、
	// 入力と出力におなじbufferが入るからそのエラー対策
	if (mSlot != -1)
	{
		ID3D11ShaderResourceView* dummy[GBuffer::NUM] = {};
		context->PSSetShaderResources(mSlot, GBuffer::NUM, dummy);
		mSlot = -1;
	}


	// レンダーターゲット取得
	ID3D11RenderTargetView* rtv[GBuffer::NUM];
	for (int i = 0; i < NUM; ++i) rtv[i] = mTargets[i].GetRTV();

	// レンダーターゲットクリア
	float clearColor[4] = { 0,0,0,1 };
	for (int i = 0; i < GBuffer::NUM; ++i) context->ClearRenderTargetView(rtv[i], clearColor);

	// レンダーターゲット設定
	context->OMSetRenderTargets(GBuffer::NUM, rtv, FRAMEWORK.GetDepthStencilView());

}

void GBuffer::Deactivate()
{
	// ターゲットを戻す
	FRAMEWORK.SetRenderTarget();
}

void GBuffer::SetTexture(UINT startSlot)
{
	// GBufferをシェーダに渡す
	// シェーダーリソース取得
	ID3D11ShaderResourceView* srv[GBuffer::NUM];
	for (int i = 0; i < NUM; ++i) srv[i] = mTargets[i].GetSRV();

	// シェーダーリソース設定
	mSlot = startSlot;
	FRAMEWORK.GetContext()->PSSetShaderResources(mSlot, GBuffer::NUM, srv);
}
