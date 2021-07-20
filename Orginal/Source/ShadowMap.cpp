#include "ShadowMap.h"

#include "lib/Framework.h"

ShadowMap::ShadowMap()
{
	mShader = std::make_unique<Shader>();
	mShader->Load(L"Shaders/ShadowMap.fx", "VSMain", "PSMain");
}

ShadowMap::~ShadowMap()
{
	mShader->UnLoad();
}

void ShadowMap::Initialize() 
{
	ID3D11Device* device = FRAMEWORK.GetDevice();

	// 定数バッファ
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBShadow);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, NULL, mConstBuffer.GetAddressOf());
	
	// 深度ステンシル設定
	{
		// 深度ステンシル設定
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
		td.Width = SHADOWMAP_X;
		td.Height = SHADOWMAP_Y;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R24G8_TYPELESS;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		// 深度ステンシルビュー設定
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		mShadowMap.CreateDSV(&td, &dsvd);

		// SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;

		mShadowMap.CreateSRV(&srvd, true);
	}


	mTarget = Vector3::Zero();
}

void ShadowMap::Activate(const DirectX::XMFLOAT4& lightDir, int textureSlot)
{
	// レンダーターゲット有効化
	mShadowMap.Activate(textureSlot);


	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// ビューポート設定
	D3D11_VIEWPORT vp = {
		0,0,
		SHADOWMAP_X,
		SHADOWMAP_Y,
		0,1 };
	context->RSSetViewports(1, &vp);

	// 定数バッファ
	{
		DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicLH(150, 150, 0.1f, 600.0f);

		Vector3 lightPos(30.0f, 30.0f, 30.0f);
		lightPos.x *= -lightDir.x;
		//lightPos.y *= -lightDir.y;
		lightPos.z *= -lightDir.z;
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(lightPos.x, lightPos.y, lightPos.z, 1.0f);
		DirectX::XMVECTOR target = DirectX::XMVectorSet(mTarget.x, mTarget.y, mTarget.z, 1.0f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f,	0.0f, 0.0f);
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);

		// 光から見た平行投影の距離が描画される
		CBShadow cb;
		DirectX::XMStoreFloat4x4(&cb.shadowVP, view * proj);
		context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);

		// セット
		context->VSSetConstantBuffers(1, 1, mConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(1, 1, mConstBuffer.GetAddressOf());

		// サンプラー
		context->PSSetSamplers(1, 1, FRAMEWORK.GetSampler(Framework::SS_SHADOW));
	}
}

void ShadowMap::Deactivate(int textureSlot)
{
	mShadowMap.Deactivate(textureSlot);
}