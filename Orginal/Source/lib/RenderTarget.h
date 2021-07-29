#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "Vector.h"

class Shader;

class RenderTarget
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		   mRTTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRTV = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepth = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDSV = nullptr;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV = nullptr;

	Vector2 mSize;
public:
	RenderTarget() = default;
	~RenderTarget() = default;

	void Initialize(float width = 0, float height = 0, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	void Activate(UINT textureSlot = 0);
	void Deactivate(UINT textureSlot = 0);
	void SetTexture(UINT slot);

	void CreateDSV(float width, float height, DXGI_FORMAT format);

	void Render(Shader* shader) const;

	ID3D11DepthStencilView* GetDSV() const { return mDSV.Get(); }
	ID3D11RenderTargetView* GetRTV() const { return mRTV.Get(); }
	ID3D11ShaderResourceView* GetSRV() const { return mSRV.Get();  }
};

