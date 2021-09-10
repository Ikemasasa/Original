#pragma once
#include <d3d11.h>
#include <wrl.h>

#define FRAMEWORK Framework::GetInstance()

class Framework
{
public:
	enum RasterizerSettings { RS_CULL_BACK, RS_CULL_FRONT, RS_CULL_NONE, RS_WIRE, RS_MAX };
	enum DepthStencilSettings { DS_FALSE, DS_TRUE, DS_MAX };
	enum SapmlerStateSettings { SS_CLAMP, SS_WRAP, SS_MAX };

private:
	Microsoft::WRL::ComPtr<ID3D11Device>		   mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	   mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>		   mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizer[RS_MAX];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState[DS_MAX];
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState[SS_MAX];

	Framework();
	~Framework();

	// èâä˙âªä÷êîåQ
	HRESULT CreateDeviceAndSwapChain(HWND hwnd);
	HRESULT CreateRasterizerStates();
	HRESULT CreateDepthStencilStates();
	HRESULT CreateSapmler();
	HRESULT SetBackBuffer();

public:
	static Framework& GetInstance()
	{
		static Framework instance;
		return instance;
	}
	ID3D11Device* GetDevice() { return mDevice.Get(); }
	ID3D11DeviceContext* GetContext();
	ID3D11RasterizerState* GetRasterizer(RasterizerSettings index) { return mRasterizer[index].Get(); }
	ID3D11DepthStencilState* GetDepthStencilState(DepthStencilSettings index) { return mDepthStencilState[index].Get(); }
	ID3D11SamplerState** GetSampler(SapmlerStateSettings index) { return mSamplerState[index].GetAddressOf(); }

	ID3D11DepthStencilView* GetDepthStencilView() { return mDepthStencilView.Get(); }

	bool Initialize(HWND hwnd);
	void Clear();
	void ScreenFlip();
	void GenerateScrshot(const wchar_t* filename);
	void SetRenderTarget();
	void ResetParam();
};

