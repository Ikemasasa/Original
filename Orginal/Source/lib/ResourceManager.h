
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
                refNum = 0;
                srv->Release();
                srv = nullptr;
            }
        }
    };

    static std::list<ResourceShaderResourceViews> mSRVs;

    // ïÅí ÇÃâÊëúì«Ç›çûÇ›
    static bool LoadWICTexture(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** outSRV, D3D11_TEXTURE2D_DESC* outDesc);

    // HDRâÊëúì«Ç›çûÇ›
    static void HDR_CheckHeader(FILE* fp, int* width, int* height);
    static bool HDR_ReadLine(BYTE* scanline, FILE* fp, int width);
    static bool HDR_ReadPixels(FILE* fp, float* buf, int width, int height);
    static bool LoadHDRTexture(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** outSRV, D3D11_TEXTURE2D_DESC* outDesc);
public:
    static ShaderManager mShaderManager;

public:
    static void Release();
    static HRESULT LoadShaderResource(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** outSRV, D3D11_TEXTURE2D_DESC* outDesc);
    static HRESULT CreateDummyTexture(ID3D11Device* device, ID3D11ShaderResourceView** srv, D3D11_TEXTURE2D_DESC* tex2dDesc);
    static void ReleaseShaderResourceView(ID3D11ShaderResourceView* SRView);
};