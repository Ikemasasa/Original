#pragma once
#include <d3d11.h>

class Blend
{
public:
    enum BLEND_MODE
    {
        NONE = 0,
        ALPHA,
        ADD,
        SUB,
        REPLACE,
        MULTIPLY,
        LIGHTEN,
        DARKEN,
        SCREEN,
        MODE_MAX
    };

private:
    static ID3D11BlendState* blendState[MODE_MAX];
    static bool bLoad;
    static BLEND_MODE enumMode;

public:
    Blend() {};
    ~Blend() { Release(); }
    static HRESULT Init(ID3D11Device* device);
    static void Release()
    {
        for (auto& it : blendState)
        {
            if (it)
            {
                it->Release();
                it = nullptr;
            }
        }
        bLoad = false;
    }

    static void Set(ID3D11DeviceContext* context, BLEND_MODE mode = NONE)
    {
        if (!bLoad) return;
        if (mode < NONE || mode >= MODE_MAX) return;
        if (mode == enumMode) return;

        context->OMSetBlendState(blendState[mode], nullptr, 0xffffffff);
        enumMode = mode;
    }

};