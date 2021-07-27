#include "ShaderManager.h"

#include <d3dcompiler.h>

#include "Framework.h"

bool ShaderManager::Compile(LPCWSTR filename, LPCSTR method, LPCSTR shadermodel, ID3DBlob** blobOut)
{
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* error = NULL;

    HRESULT hr = S_OK;
    hr = D3DCompileFromFile(
        filename,
        NULL,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        method,
        shadermodel,
        shaderFlags,
        0,
        blobOut,
        &error
    );

    // エラー出力
    if (error != NULL)
    {
        OutputDebugStringA("-------------------------------------------------------\n");
        OutputDebugStringA((char*)error->GetBufferPointer());
        OutputDebugStringA("-------------------------------------------------------\n");
        error->Release();
        error = NULL;

        return false;
    }

	return true;
}

bool ShaderManager::LoadVertexShader(LPCWSTR filename, LPCSTR func, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, int numElements, ID3D11VertexShader** outVS, ID3D11InputLayout** outIL, LPCSTR shadermodel)
{
    // なんか入ってたらfales
    if (*outVS != NULL) return false;
    if (*outIL != NULL) return false;


    // すでに読み込みしているかチェック
    ResourceVertexShader* find = nullptr;
    for (auto& vs : mVertexShaders)
    {
        if (wcscmp(vs.path.c_str(), filename) != 0) continue;
        if (strcmp(vs.func.c_str(), func) != 0) continue;

        // あった
        find = &vs;
        break;
    }

    // なかったら
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceVertexShader push;

        // 頂点シェーダ
        ID3DBlob* blob = nullptr;
        hr = Compile(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &push.vs);
        if (FAILED(hr))
        {
            blob->Release();
            return false;
        }

        // 入力レイアウト
        hr = device->CreateInputLayout(
            inputElementDesc,
            numElements,
            blob->GetBufferPointer(),
            blob->GetBufferSize(),
            &push.layout
        );

        blob->Release();
        if (FAILED(hr)) return false;

        // ファイルパス、関数名保存
        push.path = filename;
        push.func = func;

        mVertexShaders.emplace_back(push);
        find = &mVertexShaders.back();
    }

    // 代入
    *outVS = find->vs;
    *outIL = find->layout;
    ++find->refNum;

    return true;
}

bool ShaderManager::LoadPixelShader(LPCWSTR filename, LPCSTR func, ID3D11PixelShader** outPS, LPCSTR shadermodel)
{
    if (*outPS != NULL) return false;


    // 探査 
    ResourceShader<ID3D11PixelShader*>* find;
    for (auto& ps : mPixelShaders)
    {
        if (wcscmp(ps.path.c_str(), filename) != 0) continue;
        if (strcmp(ps.func.c_str(), func) != 0) continue;

        find = &ps;
        break;
    }

    // 見つからなかったら
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceShader<ID3D11PixelShader*> push;

        // ピクセルシェーダ
        ID3DBlob* blob = nullptr;
        hr = Compile(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &push.shader);
        blob->Release();
        if (FAILED(hr)) return false;

        // ファイルパス、関数名保存
        push.path = filename;
        push.func = func;

        mPixelShaders.emplace_back(push);
        find = &mPixelShaders.back();
    }

    // 代入
    *outPS = find->shader;
    ++find->refNum;

    return true;

}

bool ShaderManager::LoadGeometryShader(LPCWSTR filename, LPCSTR func, ID3D11GeometryShader** outGS, LPCSTR shadermodel)
{
    if (*outGS != NULL) return false;


    // 探査 
    ResourceShader<ID3D11GeometryShader*>* find;
    for (auto& gs : mGeometryShaders)
    {
        if (wcscmp(gs.path.c_str(), filename) != 0) continue;
        if (strcmp(gs.func.c_str(), func) != 0) continue;

        find = &gs;
        break;
    }

    // 見つからなかったら
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceShader<ID3D11GeometryShader*> push;

        // ピクセルシェーダ
        ID3DBlob* blob = nullptr;
        hr = Compile(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &push.shader);
        blob->Release();
        if (FAILED(hr)) return false;

        // ファイルパス、関数名保存
        push.path = filename;
        push.func = func;

        mGeometryShaders.emplace_back(push);
        find = &mGeometryShaders.back();
    }

    // 代入
    *outGS = find->shader;
    ++find->refNum;

    return true;
}

bool ShaderManager::LoadComputeShader(LPCWSTR filename, LPCSTR func, ID3D11ComputeShader** outCS, LPCSTR shadermodel)
{
    if (*outCS != NULL) return false;


    // 探査 
    ResourceShader<ID3D11ComputeShader*>* find;
    for (auto& cs : mComputeShaders)
    {
        if (wcscmp(cs.path.c_str(), filename) != 0) continue;
        if (strcmp(cs.func.c_str(), func) != 0) continue;

        find = &cs;
        break;
    }

    // 見つからなかったら
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceShader<ID3D11ComputeShader*> push;

        // ピクセルシェーダ
        ID3DBlob* blob = nullptr;
        hr = Compile(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &push.shader);
        blob->Release();
        if (FAILED(hr)) return false;

        // ファイルパス、関数名保存
        push.path = filename;
        push.func = func;

        mComputeShaders.emplace_back(push);
        find = &mComputeShaders.back();
    }

    // 代入
    *outCS = find->shader;
    ++find->refNum;

    return true;
}

void ShaderManager::ReleaseVertexShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* inputLayout)
{
    if (!vertexShader)return;
    if (!inputLayout)return;

    for (auto it = mVertexShaders.begin(); it != mVertexShaders.end();)
    {
        if (it->vs != vertexShader) { ++it; continue; }
        if (it->layout != inputLayout) { ++it; continue; }

        it->Release();
        if (it->refNum <= 0)
        {
            // リストから消す
            mVertexShaders.erase(it);
        }

        break;
    }
}

void ShaderManager::ReleasePixelShader(ID3D11PixelShader* pShader)
{
    if (!pShader)return;

    for (auto it = mPixelShaders.begin(); it != mPixelShaders.end();)
    {
        if (it->shader != pShader) { ++it; continue; }

        it->Release();
        if (it->refNum <= 0)
        {
            // リストから消す
            mPixelShaders.erase(it);
        }

        break;
    }

}

void ShaderManager::ReleaseGeometryShader(ID3D11GeometryShader* pShader)
{
    if (!pShader)return;

    for (auto it = mGeometryShaders.begin(); it != mGeometryShaders.end();)
    {
        if (it->shader != pShader) { ++it; continue; }

        it->Release();
        if (it->refNum <= 0)
        {
            // リストから消す
            mGeometryShaders.erase(it);
        }

        break;
    }
}

void ShaderManager::ReleaseComputeShader(ID3D11ComputeShader* pShader)
{
    if (!pShader)return;

    for (auto it = mComputeShaders.begin(); it != mComputeShaders.end();)
    {
        if (it->shader != pShader) { ++it; continue; }

        it->Release();
        if (it->refNum <= 0)
        {
            // リストから消す
            mComputeShaders.erase(it);
        }

        break;
    }
}
