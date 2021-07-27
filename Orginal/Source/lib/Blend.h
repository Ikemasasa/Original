#pragma once
#include <d3d11.h>

#include "Framework.h"

class Blend
{
public:
    enum BLEND_MODE
    {
        NONE = 0,
        ALPHA,
        ADD,
        PMA,
        //SUB,
        //REPLACE,
        //MULTIPLY,
        //LIGHTEN,
        //DARKEN,
        //SCREEN,
        
        MODE_MAX
    };

private:
    static ID3D11BlendState* mBlendState[MODE_MAX];

public:
    Blend() = default;
    ~Blend() { Release(); }
    static HRESULT Init(ID3D11Device* device);
    static void Release();

    static void Set(BLEND_MODE mode = NONE);

};