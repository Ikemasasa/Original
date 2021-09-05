#include "ResourceManager.h"

#include "DirectXTK/Inc/WICTextureLoader.h"
#include "Framework.h"

using namespace std;

// staticメンバ
std::list<ResourceManager::ResourceShaderResourceViews> ResourceManager::mSRVs;
ShaderManager ResourceManager::mShaderManager;

void ResourceManager::Release()
{
    for (auto& srv : mSRVs) srv.Release(true);
    mSRVs.clear();
    mShaderManager.ReleaseAll();
}

HRESULT ResourceManager::LoadShaderResource(ID3D11Device * device, const wchar_t * filename, ID3D11ShaderResourceView ** outSRV, D3D11_TEXTURE2D_DESC* outDesc)
{
    int no = -1;
    ResourceShaderResourceViews* find = nullptr;
    ID3D11Resource* resource = nullptr;

    // すでに読み込みしているかチェック
    for (auto& srv : mSRVs)
    {
        if (wcscmp(srv.path.c_str(), filename) != 0) continue;

        // あった
        find = &srv;
        find->srv->GetResource(&resource);
        break;
    }

    HRESULT hr = S_OK;

    ID3D11Texture2D* tex2D = nullptr;
    if (!find) // なかった
    {
        ResourceShaderResourceViews push;

        hr = DirectX::CreateWICTextureFromFile(device, filename, &resource, &push.srv);
        if (FAILED(hr))
        {
            hr = CreateDummyTexture(device, &tex2D, outDesc, &push.srv);
            if (FAILED(hr)) return hr;
        }

        push.path = filename;
        mSRVs.emplace_back(push);

        find = &mSRVs.back();
    }

    if(!tex2D) resource->QueryInterface(&tex2D);
    *outSRV = find->srv;
    tex2D->GetDesc(outDesc);
    find->refNum++;

    if(tex2D)tex2D->Release();
    if(resource)resource->Release();

    return hr;
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
        if (it->refNum <= 0)
        {
            // リストから消す
            mSRVs.erase(it);
        }
        
        break;
    }
}