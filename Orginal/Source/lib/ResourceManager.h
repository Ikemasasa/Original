
#pragma once
#include <D3D11.h>
#include <list>
#include <string>

#include "ShaderManager.h"

class ResourceManager
{
    struct ResourceShaderResourceViews
    {
        int refNum = 0;
        std::wstring path;
        ID3D11ShaderResourceView* srv;
        void Release(bool bForce = false)
        {
            --refNum;
            if (refNum <= 0) bForce = true;

            if (bForce)
            {
                srv->Release();
                srv = nullptr;
            }
        }
    };

    static std::list<ResourceShaderResourceViews> mSRVs;

public:
    static ShaderManager mShaderManager;

public:
    static void Release();
    static HRESULT LoadShaderResource(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** outSRV, D3D11_TEXTURE2D_DESC* outDesc);
    static HRESULT CreateDummyTexture(ID3D11Device* device, ID3D11Texture2D** tex2d, D3D11_TEXTURE2D_DESC* tex2dDesc, ID3D11ShaderResourceView** srv);
    static void ReleaseShaderResourceView(ID3D11ShaderResourceView* SRView);
};