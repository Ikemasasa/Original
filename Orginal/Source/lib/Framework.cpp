#include "framework.h"

#include <DirectXmath.h>
#include <wincodec.h>

#include "DirectXTK/Inc/ScreenGrab.h"

#include "Blend.h"
#include "ResourceManager.h"
#include "Window.h"

Framework::Framework()
{
    mDevice           = nullptr;
    mDeviceContext    = nullptr;
    mSwapChain        = nullptr;
    mRenderTargetView = nullptr;
    mDepthStencilView = nullptr;
}

Framework::~Framework()
{
    ResourceManager::Release();
    Blend::Release();

    mDeviceContext->ClearState();
    mDeviceContext->Flush();

#if _DEBUG
    // 残ってるcomオブジェクトを出力ウィンドウに表示する
    // これを読んだ時点でのやつなので、frameworkのcomptrや、staticメンバ変数とかは表示される(0831現在はDeviceのrefcountが16)
    ID3D11Debug* debug;
    // 作成
    mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
    // 詳細表示
    debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
    debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    debug->Release();
#endif


}

HRESULT Framework::CreateDeviceAndSwapChain(HWND hwnd)
{
    HRESULT hr = S_OK;
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);


    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
    SwapChainDesc.BufferCount = 2;
    SwapChainDesc.BufferDesc.Width = static_cast<UINT>(Window::GetInstance().GetWidth());
    SwapChainDesc.BufferDesc.Height = static_cast<UINT>(Window::GetInstance().GetHeight());
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = hwnd;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.Windowed = TRUE;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    D3D_FEATURE_LEVEL FeatureLevel;
    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &SwapChainDesc, &mSwapChain, &mDevice, &FeatureLevel, &mDeviceContext);

        // 成功したら抜ける
        if (SUCCEEDED(hr)) break;
    }
    
    return hr;
}

HRESULT Framework::CreateRasterizerStates()
{
    HRESULT hr = S_OK;

    D3D11_RASTERIZER_DESC rd;
    for (int i = 0; i < RS_MAX; ++i)
    {
        switch (i)
        {
        case RS_CULL_BACK:
            ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
            rd.CullMode = D3D11_CULL_BACK;
            rd.FillMode = D3D11_FILL_SOLID;
            rd.FrontCounterClockwise = true;
            break;

        case RS_CULL_FRONT:
            ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
            rd.CullMode = D3D11_CULL_FRONT;
            rd.FillMode = D3D11_FILL_SOLID;
            rd.FrontCounterClockwise = false;
            break;

        case RS_WIRE:
            ZeroMemory(&rd, sizeof(rd));
            rd.CullMode = D3D11_CULL_BACK;
            rd.FillMode = D3D11_FILL_WIREFRAME;
            rd.FrontCounterClockwise = true;
            break;

        case RS_CULL_NONE:
            ZeroMemory(&rd, sizeof(rd));
            rd.CullMode = D3D11_CULL_NONE;
            rd.FillMode = D3D11_FILL_SOLID;
            break;
        }

        hr = mDevice->CreateRasterizerState(&rd, mRasterizer[i].GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    return hr;
}

HRESULT Framework::CreateDepthStencilStates()
{
    HRESULT hr = S_OK;

    D3D11_DEPTH_STENCIL_DESC dsd;
    for (int i = 0; i < DS_MAX; ++i)
    {
        switch (i)
        {
        case DS_TRUE:
            ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
            dsd.DepthEnable = true;
            dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            dsd.StencilEnable = false;
            dsd.DepthFunc = D3D11_COMPARISON_LESS;
            break;

        case DS_FALSE:
            ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
            dsd.DepthEnable = false;
            break;
        }

        hr = mDevice->CreateDepthStencilState(&dsd, mDepthStencilState[i].GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    return hr;
}

HRESULT Framework::CreateSapmler()
{
    HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sd;
    for (int i = 0; i < SS_MAX; ++i)
    {
        switch (i)
        {
        case SS_CLAMP:
            ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
            sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
            sd.MinLOD = 0;
            sd.MaxLOD = D3D11_FLOAT32_MAX;
            break;

        case SS_WRAP:
            ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
            sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
            sd.MinLOD = 0;
            sd.MaxLOD = D3D11_FLOAT32_MAX;
            break;
        }

        hr = mDevice->CreateSamplerState(&sd, mSamplerState[i].GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    return hr;
}

HRESULT Framework::SetBackBuffer()
{
    // レンダーターゲットにバックバッファを指定する
    HRESULT hr = S_OK;

    // スワップチェインからバックバッファを取得
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return hr;

    // バックバッファを対象としたレンダーターゲット作成
    hr = mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mRenderTargetView);
    if (pBackBuffer) pBackBuffer->Release();
    if (FAILED(hr)) return hr;


    // 深度 / ステンシルバッファ作成
    ID3D11Texture2D* pDepthStencil = nullptr;

    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = static_cast<UINT>(Window::GetInstance().GetWidth());
    descDepth.Height = static_cast<UINT>(Window::GetInstance().GetHeight());
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = mDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
    if (FAILED(hr)) return hr;

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = mDevice->CreateDepthStencilView(pDepthStencil, &descDSV, mDepthStencilView.GetAddressOf());
    if (pDepthStencil) pDepthStencil->Release();
    if (FAILED(hr)) return hr;


    // セット
    mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

    return hr;
}

ID3D11DeviceContext* Framework::GetContext()
{
    return mDeviceContext.Get();
}

bool Framework::Initialize(HWND hwnd)
{
	HRESULT hr = S_OK;

    // デバイス、スワップチェインを作成
    hr = CreateDeviceAndSwapChain(hwnd);
    if (FAILED(hr)) return false;

    // ラスタライザ作成
    hr = CreateRasterizerStates();
    if (FAILED(hr)) return false;

    // 深度ステンシル作成
    hr = CreateDepthStencilStates();
    if (FAILED(hr)) return false;

    // サンプラー作成
    hr = CreateSapmler();
    if (FAILED(hr)) return false;

    // ブレンド初期化
    hr = Blend::Init(mDevice.Get());
    if (FAILED(hr)) return false;

    // 描画先をバックバッファに設定
    hr = SetBackBuffer();
    if (FAILED(hr)) return false;

    // ビューポートの設定
    D3D11_VIEWPORT viewport;
    viewport.Width = Window::GetInstance().GetWidth();
    viewport.Height = Window::GetInstance().GetHeight();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    mDeviceContext->RSSetViewports(1, &viewport);

    return true;

}

void Framework::Clear()
{
    // バックバッファをクリア
    float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), ClearColor);
    mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f, 0);
    mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

    // アルファブレンドをセット
    Blend::Set(Blend::BLEND_MODE::ALPHA);

    // 各種デフォルトを設定
    ResetParam();
}

void Framework::ScreenFlip()
{
    mSwapChain->Present(0, 0);
}

void Framework::GenerateScrshot(const wchar_t* filename)
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = S_OK;

    // バックバッファを取得
    hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
    if (FAILED(hr)) return;

    // スクショ作成
    hr = DirectX::SaveWICTextureToFile(mDeviceContext.Get(), backBuffer.Get(), GUID_ContainerFormatPng, filename);
    if (FAILED(hr)) return;
}

void Framework::SetRenderTarget()
{
    mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

    // ビューポートの設定
    D3D11_VIEWPORT viewport;
    viewport.Width = Window::GetInstance().GetWidth();
    viewport.Height = Window::GetInstance().GetHeight();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    mDeviceContext->RSSetViewports(1, &viewport);
}

void Framework::ResetParam()
{
    mDeviceContext->OMSetDepthStencilState(mDepthStencilState[DS_TRUE].Get(), 0);
    mDeviceContext->RSSetState(mRasterizer[RS_CULL_BACK].Get());
    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ID3D11SamplerState* ss[] = {
        mSamplerState[SS_CLAMP].Get(),
        mSamplerState[SS_WRAP].Get()
    };
    mDeviceContext->PSSetSamplers(0, 2, ss);
}
