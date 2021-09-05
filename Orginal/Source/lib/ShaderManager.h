#pragma once

#include <D3D11.h>
#include <list>

#include <string>

class ShaderManager
{
    friend class ResourceManager;

private:
    // レイアウトがあるからVSだけ別にする
    struct ResourceVertexShader
    {
        int refNum = 0;
        ID3D11VertexShader* vs = nullptr;
        ID3D11InputLayout* layout = nullptr;
        std::wstring path;
        std::string func;
        void Release(bool force = false)
        {
            --refNum;
            if (refNum <= 0) force = true;

            if (force)
            {
                vs->Release();
                layout->Release();
                vs = nullptr;
                layout = nullptr;
            }
        }

    };

    template<class T>
    struct ResourceShader
    {
        int refNum = 0;
        T*   shader = nullptr;
        std::wstring path;
        std::string func;

        void Release(bool force = false)
        {
            --refNum;
            if (refNum <= 0) force = true;

            if (force)
            {
                shader->Release();
                shader = nullptr;
            }
        }
    };

    std::list<ResourceVertexShader>                 mVertexShaders;
    std::list<ResourceShader<ID3D11PixelShader>>    mPixelShaders;
    std::list<ResourceShader<ID3D11GeometryShader>> mGeometryShaders;
    std::list<ResourceShader<ID3D11ComputeShader>>  mComputeShaders;

    // 実態作成禁止(フレンドクラスだけ)
    ShaderManager() = default;
    ~ShaderManager() = default;

    bool Compile(LPCWSTR filename, LPCSTR method, LPCSTR shadermodel, ID3DBlob** blobOut);
public:
    bool LoadVertexShader(LPCWSTR filename, LPCSTR func, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, int numElements, ID3D11VertexShader** outVS, ID3D11InputLayout** outIL, LPCSTR shadermodel);
    bool LoadPixelShader(LPCWSTR filename, LPCSTR func, ID3D11PixelShader** outPS, LPCSTR shadermodel);
    bool LoadGeometryShader(LPCWSTR filename, LPCSTR func, ID3D11GeometryShader** outGS, LPCSTR shadermodel);
    bool LoadComputeShader(LPCWSTR filename, LPCSTR func, ID3D11ComputeShader** outCS, LPCSTR shadermodel);

    void ReleaseAll();
    void ReleaseVertexShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* inputLayout);
    void ReleasePixelShader(ID3D11PixelShader* pShader);
    void ReleaseGeometryShader(ID3D11GeometryShader* pShader);
    void ReleaseComputeShader(ID3D11ComputeShader* pShader);
};