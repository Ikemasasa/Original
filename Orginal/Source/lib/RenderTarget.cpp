#include "RenderTarget.h"

#include "Framework.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Window.h"

void RenderTarget::Initialize(float width, float height, DXGI_FORMAT format)
{
	ID3D11Device* device = FRAMEWORK.GetDevice();

	if (width == 0.0f || height == 0.0f)
	{
		width = Window::GetInstance().GetWidth();
		height = Window::GetInstance().GetHeight();
	}

	{
		// レンダーターゲット設定
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
		td.Width = static_cast<UINT>(width);
		td.Height = static_cast<UINT>(height);
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = format;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		// テクスチャ生成
		HRESULT hr = device->CreateTexture2D(&td, NULL, mRTTexture.GetAddressOf());
		if (FAILED(hr)) return;

		//	レンダーターゲットビュー
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		hr = device->CreateRenderTargetView(mRTTexture.Get(), &rtvDesc, mRTV.GetAddressOf());
	
		// シェーダリソースビューの設定
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = rtvDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		// シェーダリソースビューの生成
		hr = device->CreateShaderResourceView(mRTTexture.Get(), &srvDesc, mSRV.GetAddressOf());
		if (FAILED(hr)) return;
	}

	{
		// 深度ステンシル設定
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
		td.Width = static_cast<UINT>(width);
		td.Height = static_cast<UINT>(height);
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		// 深度ステンシルテクスチャ生成
		HRESULT hr = device->CreateTexture2D(&td, NULL, mDepth.GetAddressOf());
		if (FAILED(hr)) return;

		// 深度ステンシルビュー設定
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dsvd.Format = td.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		// 深度ステンシルビュー生成
		hr = device->CreateDepthStencilView(mDepth.Get(), &dsvd, mDSV.GetAddressOf());
		if (FAILED(hr)) return;
	}
}

void RenderTarget::Activate()
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// ダミーSRVをセット(エラー対策)
	if (mTextureSlot != -1)
	{
		ID3D11ShaderResourceView* dummy = nullptr;
		context->PSSetShaderResources(mTextureSlot, 1, &dummy);
		mTextureSlot = -1;
	}


	// レンダーターゲットビュー設定
	float clearColor[4] = { 0, 0, 0, 1 };
	if (mRTV.Get()) context->ClearRenderTargetView(mRTV.Get(), clearColor);
	if (mDSV.Get()) context->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (mRTV.Get())
	{
		ID3D11RenderTargetView* targets[] = {
			mRTV.Get(),
		};

		context->OMSetRenderTargets( 1, targets, mDSV.Get());
	}
	else
	{
		context->OMSetRenderTargets(0, nullptr, mDSV.Get());
	}


	// ターゲットサイズ取得
	D3D11_TEXTURE2D_DESC desc = {};
	mRTTexture->GetDesc(&desc);

	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)desc.Width;
	viewport.Height = (FLOAT)desc.Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);
}

void RenderTarget::Deactivate()
{
	FRAMEWORK.SetRenderTarget();
}

void RenderTarget::Render(Shader* shader) const
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	{
		// ターゲットサイズ取得
		D3D11_TEXTURE2D_DESC desc = {};
		mRTTexture->GetDesc(&desc);

		// 描画
		UINT num = 1;
		D3D11_VIEWPORT vp;
		context->RSGetViewports(&num, &vp);

		Vector2 pos(0.0f, 0.0f);
		Vector2 scale(vp.Width / desc.Width, vp.Height / desc.Height);
		Vector2 texPos(0.0f, 0.0f);
		Vector2 size(desc.Width, desc.Height);

		if(shader) Renderer2D::GetInstance().Render(mSRV.Get(), shader, pos, scale, texPos, size);
		else	   Renderer2D::GetInstance().Render(mSRV.Get(), pos, scale, texPos, size);
	}
}

void RenderTarget::SetTexture(UINT slot)
{
	mTextureSlot = slot;
	FRAMEWORK.GetContext()->PSSetShaderResources(mTextureSlot, 1, mSRV.GetAddressOf());
}

void RenderTarget::CreateDSV(float width, float height, DXGI_FORMAT format)
{
	HRESULT hr = S_OK;

	ID3D11Device* device = FRAMEWORK.GetDevice();

	// シャドウマップ用、必要になったら色々変えようかな

	// 深度ステンシル設定
	{

		// 深度ステンシルテクスチャ生成
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
		td.Width = static_cast<UINT>(width);
		td.Height = static_cast<UINT>(height);
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = format;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		device->CreateTexture2D(&td, NULL, mDepth.GetAddressOf());

		// 深度ステンシルビュー生成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;\
		device->CreateDepthStencilView(mDepth.Get(), &dsvd, mDSV.GetAddressOf());

		// SRV作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		device->CreateShaderResourceView(mDepth.Get(), &srvd, mSRV.GetAddressOf());
	}
}

void RenderTarget::CreateDSResourceView()
{
	D3D11_TEXTURE2D_DESC desc = {};
	mDepth->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.Format = desc.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	FRAMEWORK.GetDevice()->CreateShaderResourceView(mDepth.Get(), &srvd, mDSResourceView.GetAddressOf());
}

