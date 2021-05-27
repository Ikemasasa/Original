#include "ResourceManager.h"

#include <d3dcompiler.h>
#include <crtdbg.h>
#include <D3D11.h>
#include <stdio.h>
#include <wchar.h>
#include <wrl.h>

#include "DirectXTK/Inc/WICTextureLoader.h"
#include "Framework.h"

using namespace std;
std::list<ResourceManager::ResourceShaderResourceViews> ResourceManager::mSRVs;
std::list<ResourceManager::ResourceVertexShaders>       ResourceManager::mVSs;
std::list<ResourceManager::ResourcePixelShaders>        ResourceManager::mPSs;
std::list<ResourceManager::ResourceComputeShaders>      ResourceManager::mCSs;

HRESULT ResourceManager::CompileShader(LPCWSTR filename, LPCSTR method, LPCSTR shaderModel, ID3DBlob** blobOut)
{
    HRESULT hr = S_OK;
    
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* error = NULL;

    hr = D3DCompileFromFile(
        filename,
        NULL,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        method,
        shaderModel,
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
    }

    return hr;

}

void ResourceManager::Release()
{
    for (auto& srv : mSRVs) srv.Release(true);
    for (auto& vs : mVSs) vs.Release(true);
    for (auto& ps : mPSs) ps.Release(true);
}

HRESULT ResourceManager::LoadShaderResource(ID3D11Device * device, const wchar_t * filename, ID3D11ShaderResourceView ** outSRV, D3D11_TEXTURE2D_DESC* outDesc)
{
    int no = -1;
    ResourceShaderResourceViews* find = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Resource> resource;

    // すでに読み込みしているかチェック
    for (auto& srv : mSRVs)
    {
        if (wcscmp(srv.path, filename) != 0) continue;

        // あった
        find = &srv;
        find->srv->GetResource(resource.GetAddressOf());
        break;
    }

    HRESULT hr = S_OK;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
    if (!find) // なかった
    {
        ResourceShaderResourceViews push;

        hr = DirectX::CreateWICTextureFromFile(device, filename, &resource, &push.srv);
        if (FAILED(hr))
        {
            hr = CreateDummyTexture(device, tex2D.GetAddressOf(), outDesc, &push.srv);
            if (FAILED(hr)) return false;
        }
        wcscpy_s(push.path, STR_MAX, filename);
        mSRVs.emplace_back(push);

        find = &mSRVs.back();
    }

    if(!tex2D) resource->QueryInterface(tex2D.GetAddressOf());
    *outSRV = find->srv;
    tex2D->GetDesc(outDesc);
    find->iRefNum++;
    return hr;
}

bool ResourceManager::LoadVertexShader(LPCWSTR filename, LPCSTR func, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, int numElements, ID3D11VertexShader** outVS, ID3D11InputLayout** outIL, LPCSTR shadermodel)
{
    // 初期化
    if (*outVS != NULL) return false;
    if (*outIL != NULL) return false;


    // すでに読み込みしているかチェック
    ResourceVertexShaders* find = NULL;
    for (auto& vs : mVSs)
    {
        if (wcscmp(vs.path, filename) != 0) continue;
        if (strcmp(vs.func, func) != 0) continue;

        // あった
        find = &vs;
        break;
    }

    // なかったら
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceVertexShaders push;

        // 頂点シェーダ
        ID3DBlob* blob = nullptr;
        hr = CompileShader(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &push.vs);
        if (FAILED(hr))
        {
            blob->Release();
            return hr;
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
        wcscpy_s(push.path, STR_MAX, filename);
        strcpy_s(push.func, FUNCSTR_MAX, func);

        mVSs.emplace_back(push);
        find = &mVSs.back();
    }

    // 代入
    *outVS = find->vs;
    *outIL = find->layout;
    ++find->iRefNum;

    return true;
}

bool ResourceManager::LoadPixelShader(LPCWSTR filename, LPCSTR func, ID3D11PixelShader** outPS, LPCSTR shadermodel)
{
    if (*outPS != NULL) return false;


    // 探査 
    ResourcePixelShaders* find = NULL;
    for (auto& ps : mPSs)
    {
        if (wcscmp(ps.path, filename) != 0) continue;
        if (strcmp(ps.func, func) != 0) continue;

        find = &ps;
        break;
    }

    // 見つからなかったら
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourcePixelShaders push;

        // ピクセルシェーダ
        ID3DBlob* blob = nullptr;
        hr = CompileShader(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &push.ps);
        blob->Release();
        if (FAILED(hr)) return false;

        // ファイルパス、関数名保存
        wcscpy_s(push.path, STR_MAX, filename);
        strcpy_s(push.func, FUNCSTR_MAX, func);

        mPSs.emplace_back(push);
        find = &mPSs.back();
    }

    // 代入
    *outPS = find->ps;
    ++find->iRefNum;

    return true;
}

bool ResourceManager::LoadComputeShader(LPCWSTR filename, LPCSTR func, ID3D11ComputeShader** outCS, LPCSTR shadermodel)
{
    if (*outCS != NULL) return false;

    ID3D11Device* device = FRAMEWORK.GetDevice();

    ResourceComputeShaders* find = NULL;
    // 探査 
    for (auto& cs : mCSs)
    {
        if (wcscmp(cs.path, filename) != 0) continue;
        if (strcmp(cs.func, func) != 0) continue;

        find = &cs;
        break;
    }

    // 見つからなかったら
    if (!find)
    {
        HRESULT hr = S_OK;
        ResourceComputeShaders push;

        ID3DBlob* blob = nullptr;
        hr = CompileShader(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &push.cs);
        blob->Release();
        if (FAILED(hr))return false;

        // ファイルパス、関数名保存
        wcscpy_s(push.path, STR_MAX, filename);
        strcpy_s(push.func, FUNCSTR_MAX, func);

        mCSs.emplace_back(push);
        find = &mCSs.back();
    }

    *outCS = find->cs;
    ++find->iRefNum;
    return true;
}

HRESULT ResourceManager::CreateDummyTexture(ID3D11Device* device, ID3D11Texture2D** tex2d, D3D11_TEXTURE2D_DESC* tex2dDesc, ID3D11ShaderResourceView** srv)
{
    HRESULT hr = S_OK;

    const UINT colorcode = 0xffffffff;
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = &colorcode;
    initData.SysMemPitch = sizeof(UINT);
    initData.SysMemSlicePitch = 0;

    ZeroMemory(tex2dDesc, sizeof(D3D11_TEXTURE2D_DESC));
    tex2dDesc->Width = tex2dDesc->Height = 1;
    tex2dDesc->MipLevels = 1;
    tex2dDesc->ArraySize = 1;
    tex2dDesc->Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tex2dDesc->SampleDesc.Count = 1;
    tex2dDesc->Usage = D3D11_USAGE_DEFAULT;
    tex2dDesc->BindFlags = D3D11_BIND_SHADER_RESOURCE;
    tex2dDesc->CPUAccessFlags = 0;
    hr = device->CreateTexture2D(tex2dDesc, &initData, tex2d);
    if (FAILED(hr)) return hr;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = tex2dDesc->Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = tex2dDesc->MipLevels;
    hr = device->CreateShaderResourceView(*tex2d, &srvDesc, srv);
    if (FAILED(hr)) return hr;

    return hr;
}

void ResourceManager::ReleaseShaderResourceView(ID3D11ShaderResourceView * SRView)
{
    if (!SRView)return;

    for (auto it = mSRVs.begin(); it != mSRVs.end();)
    {
        if (it->srv != SRView) { ++it; continue; }

        it->Release();
        if (it->iRefNum <= 0)
        {
            // リストから消す
            mSRVs.erase(it);
        }
        
        break;
    }
}

void ResourceManager::ReleaseVertexShader(ID3D11VertexShader * vertexShader, ID3D11InputLayout * inputLayout)
{
    if (!vertexShader)return;
    if (!inputLayout)return;

    for (auto it = mVSs.begin(); it != mVSs.end();)
    {
        if (it->vs != vertexShader)    { ++it; continue; }
        if (it->layout != inputLayout) { ++it; continue; }

        it->Release();
        if (it->iRefNum <= 0)
        {
            // リストから消す
            mVSs.erase(it);
        }

        break;
    }
}

void ResourceManager::ReleasePixelShader(ID3D11PixelShader * pixelShader)
{
    if (!pixelShader)return;

    for (auto it = mPSs.begin(); it != mPSs.end();)
    {
        if (it->ps != pixelShader) { ++it; continue; }

        it->Release();
        if (it->iRefNum <= 0)
        {
            // リストから消す
            mPSs.erase(it);
        }
        
        break;
    }

}

void ResourceManager::ReleaseComputeShader(ID3D11ComputeShader* pShader)
{
    if (!pShader)return;

    for (auto it = mCSs.begin(); it != mCSs.end();)
    {
        if (it->cs != pShader) { ++it; continue; }

        it->Release();
        if (it->iRefNum <= 0)
        {
            // リストから消す
            mCSs.erase(it);
        }

        break;
    }
}