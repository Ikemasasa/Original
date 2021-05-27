#pragma once
#include <d3d11.h>
#include <wrl.h>

class Shader;

class RenderTarget
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		   mRTTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRTV = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepth = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDSV = nullptr;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV = nullptr;


public:
	RenderTarget() = default;
	~RenderTarget() = default;

	void Initialize(float width = 0, float height = 0);
	void Activate(UINT textureSlot = 0);
	void Deactivate(UINT textureSlot = 0);
	void SetTexture(UINT slot);

	void CreateRTV(D3D11_TEXTURE2D_DESC* td, D3D11_RENDER_TARGET_VIEW_DESC* rtvd);
	void CreateDSV(D3D11_TEXTURE2D_DESC* td, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvd);
	void CreateSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvd, bool isDepth = false);

	void Render(Shader* shader);
};

