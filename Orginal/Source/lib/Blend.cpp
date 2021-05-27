#include <d3d11.h>
#include "blend.h"
ID3D11BlendState* Blend::blendState[Blend::MODE_MAX] = {};
bool Blend::bLoad = false;
Blend::BLEND_MODE Blend::enumMode = Blend::NONE;

struct BLEND_DATA
{
    D3D11_BLEND    srcBlend;
    D3D11_BLEND    destBlend;
    D3D11_BLEND_OP blendOp;
    D3D11_BLEND    srcBlendAlpha;
    D3D11_BLEND    destBlendAlpha;
    D3D11_BLEND_OP blendOpAlpha;
};

BLEND_DATA blendData[Blend::MODE_MAX] =
{
    // NONE
    {
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_INV_SRC_ALPHA,
        D3D11_BLEND_OP_ADD,
        D3D11_BLEND_ONE,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_OP_ADD
    },

    // ALPHA
    {
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_INV_SRC_ALPHA,
        D3D11_BLEND_OP_ADD,
        D3D11_BLEND_ONE,
        D3D11_BLEND_INV_SRC_ALPHA,
        D3D11_BLEND_OP_ADD
    },

    // ADD
    {
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_ONE,
        D3D11_BLEND_OP_ADD,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_ONE,
        D3D11_BLEND_OP_ADD
    },

    // SUB
    {
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_ONE,
        D3D11_BLEND_OP_REV_SUBTRACT,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_ONE,
        D3D11_BLEND_OP_ADD
    },

    // REPLACE
    {
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_OP_ADD,
        D3D11_BLEND_ONE,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_OP_ADD
    },

    // MULTIPLY
    {
        D3D11_BLEND_DEST_COLOR,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_OP_ADD,
        D3D11_BLEND_DEST_ALPHA,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_OP_ADD
    },

    // LIGHTEN
    {
        D3D11_BLEND_ONE,
        D3D11_BLEND_ONE,
        D3D11_BLEND_OP_MAX,
        D3D11_BLEND_ONE,
        D3D11_BLEND_ONE,
        D3D11_BLEND_OP_MAX
    },

    // DARKEN
    {
        D3D11_BLEND_ONE,
        D3D11_BLEND_ONE,
        D3D11_BLEND_OP_MIN,
        D3D11_BLEND_ONE,
        D3D11_BLEND_ONE,
        D3D11_BLEND_OP_MIN
    },

    // SCREEN
    {
        D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_INV_SRC_ALPHA,
        D3D11_BLEND_OP_ADD,
        D3D11_BLEND_ONE,
        D3D11_BLEND_INV_SRC_ALPHA,
        D3D11_BLEND_OP_ADD
    },
};

HRESULT Blend::Init(ID3D11Device * device)
{
    HRESULT hr = S_OK;
    D3D11_BLEND_DESC blendDesc = {};

    for (int i = 0; i < MODE_MAX; ++i)
    {
        ZeroMemory(&blendDesc, sizeof(blendDesc));
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.IndependentBlendEnable = false;
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend  = blendData[i].srcBlend;
        blendDesc.RenderTarget[0].DestBlend = blendData[i].destBlend;
        blendDesc.RenderTarget[0].BlendOp   = blendData[i].blendOp;
        blendDesc.RenderTarget[0].SrcBlendAlpha = blendData[i].srcBlendAlpha;
        blendDesc.RenderTarget[0].DestBlendAlpha = blendData[i].destBlendAlpha;
        blendDesc.RenderTarget[0].BlendOpAlpha = blendData[i].blendOpAlpha;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        hr = device->CreateBlendState(&blendDesc, &blendState[i]);
        if (FAILED(hr))
        {
            bLoad = false;
            return hr;
        }
    }
    bLoad = true;
    return hr;
}
