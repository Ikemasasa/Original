
#pragma once
#include <D3D11.h>
#include <list>

#include "ShaderManager.h"

class ResourceManager
{
private:
    static const int RESOURCE_MAX = 64;
    static const int STR_MAX = 128;
    static const int FUNCSTR_MAX = 32;

    struct ResourceShaderResourceViews
    {
        int iRefNum;
        WCHAR path[STR_MAX];
        ID3D11ShaderResourceView* srv;
        ResourceShaderResourceViews() : iRefNum(0), srv(NULL) { path[0] = '\0'; }
        void Release(bool bForce = false)
        {
            if (--iRefNum <= 0) bForce = true;

            if (bForce)
            {
                srv->Release();
                srv = NULL;
            }
        }
    };

    static std::list<ResourceShaderResourceViews> mSRVs;
    static ShaderManager mShaderManager;


    static HRESULT CompileShader(LPCWSTR filename, LPCSTR method, LPCSTR shaderModel, ID3DBlob** blobOut);
public:
    static void Release();
    static HRESULT LoadShaderResource(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** outSRV, D3D11_TEXTURE2D_DESC* outDesc);
    static bool LoadVertexShader(LPCWSTR filename, LPCSTR func, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, int numElements, ID3D11VertexShader** outVS, ID3D11InputLayout** outIL, LPCSTR shadermodel);
    static bool LoadPixelShader(LPCWSTR filename, LPCSTR func, ID3D11PixelShader** outPS, LPCSTR shadermodel);
    static bool LoadComputeShader(LPCWSTR filename, LPCSTR func, ID3D11ComputeShader** outCS, LPCSTR shadermodel);
    static HRESULT CreateDummyTexture(ID3D11Device* device, ID3D11Texture2D** tex2d, D3D11_TEXTURE2D_DESC* tex2dDesc, ID3D11ShaderResourceView** srv);

    static void ReleaseShaderResourceView(ID3D11ShaderResourceView* SRView);
    static void ReleaseVertexShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* inputLayout);
    static void ReleasePixelShader(ID3D11PixelShader* pShader);
    static void ReleaseComputeShader(ID3D11ComputeShader* pShader);
};