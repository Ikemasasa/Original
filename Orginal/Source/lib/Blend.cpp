#include <d3d11.h>
#include "blend.h"

// staticÉÅÉìÉoïœêî
ID3D11BlendState* Blend::mBlendState[Blend::MODE_MAX] = {};

HRESULT Blend::Init(ID3D11Device * device)
{
    HRESULT hr = S_OK;
    D3D11_BLEND_DESC blendDesc = {};
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    for (int i = 0; i < MODE_MAX; ++i)
    {
        switch (i)
        {
        case NONE:
            blendDesc.RenderTarget[0].BlendEnable = false;
            blendDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
            blendDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
            break;

        case ALPHA:
            blendDesc.RenderTarget[0].BlendEnable = true;
            blendDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
            blendDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
            break;

        case ADD:
            blendDesc.RenderTarget[0].BlendEnable = true;
            blendDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
            blendDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_ONE;
            blendDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
            break;

        case PMA:
            blendDesc.RenderTarget[0].BlendEnable = true;
            blendDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
            blendDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
            break;
        }

        hr = device->CreateBlendState(&blendDesc, &mBlendState[i]);
        if (FAILED(hr)) return hr;
    }

    return hr;
}

void Blend::Release()
{
    for (auto& it : mBlendState)
    {
        if (it)
        {
            it->Release();
            it = nullptr;
        }
    }
}

void Blend::Set(BLEND_MODE mode)
{
    if (mode < NONE || mode >= MODE_MAX) return;

    FRAMEWORK.GetContext()->OMSetBlendState(mBlendState[mode], nullptr, 0xffffffff);
}
