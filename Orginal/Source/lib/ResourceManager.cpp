#include "ResourceManager.h"

#include <memory>

#include "DirectXTK/Inc/WICTextureLoader.h"
#include "DirectXTex/Inc/DirectXTex.h"

#include "ConvertString.h"
#include "Framework.h"

using namespace std;

// staticメンバ
std::list<ResourceManager::ResourceShaderResourceViews> ResourceManager::mSRVs;
ShaderManager ResourceManager::ShaderManager;

void ResourceManager::Release()
{
    for (auto& srv : mSRVs) srv.Release(true);
    mSRVs.clear();
    ShaderManager.ReleaseAll();
}

HRESULT ResourceManager::LoadShaderResource(ID3D11Device * device, const wchar_t * filename, ID3D11ShaderResourceView ** outSRV, D3D11_TEXTURE2D_DESC* outDesc)
{
    ResourceShaderResourceViews* find = nullptr;

    // すでに読み込みしているかチェック
    for (auto& srv : mSRVs)
    {
        if (wcscmp(srv.path.c_str(), filename) != 0) continue;

        // あった
        find = &srv;

        // Desc取得
        ID3D11Resource* resource = nullptr;
        ID3D11Texture2D* tex2D = nullptr;
        find->srv->GetResource(&resource);
        resource->QueryInterface(&tex2D);
        tex2D->GetDesc(outDesc);

        resource->Release();
        tex2D->Release();
        break;
    }

    if (!find) // なかった
    {
        ResourceShaderResourceViews push = {};

        // 拡張子を判定
        int cmp = _wcsicmp(&filename[wcslen(filename) - 3], L"HDR");
        
        // 判定に応じて読み込み関数を変える
        ID3D11ShaderResourceView* srv = nullptr;
        bool isLoad = false;
        if (cmp == 0) {
            isLoad = LoadHDRTexture(device, filename, &push.srv, outDesc);
        }
        else {
            isLoad = LoadWICTexture(device, filename, &push.srv, outDesc);
        }

        if (isLoad)
        {
            // データ保存
            push.path = filename;
            mSRVs.emplace_back(push);

            find = &mSRVs.back();
        }
    }

    // 引数の戻り値に代入
    *outSRV = find->srv;
    // outDesc; 上のどこかで代入してる
    find->refNum++;

    return S_OK;
}

HRESULT ResourceManager::CreateDummyTexture(ID3D11Device* device, ID3D11ShaderResourceView** srv, D3D11_TEXTURE2D_DESC* tex2dDesc)
{
    HRESULT hr = S_OK;

    ID3D11Texture2D* tex2D = nullptr;

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
    hr = device->CreateTexture2D(tex2dDesc, &initData, &tex2D);
    if (FAILED(hr)) return hr;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = tex2dDesc->Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = tex2dDesc->MipLevels;
    hr = device->CreateShaderResourceView(tex2D, &srvDesc, srv);
    if (FAILED(hr)) return hr;

    if(tex2D)tex2D->Release();

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

bool ResourceManager::LoadWICTexture(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** outSRV, D3D11_TEXTURE2D_DESC* outDesc)
{
    HRESULT hr = S_OK;

    ID3D11Resource* resource = nullptr;
    hr = DirectX::CreateWICTextureFromFile(device, filename, &resource, outSRV);
    if (SUCCEEDED(hr))
    {
        // desc取得
        ID3D11Texture2D* tex2D = nullptr;
        resource->QueryInterface(&tex2D);
        tex2D->GetDesc(outDesc);

        tex2D->Release();
        resource->Release();
    }
    else
    {
        // ダミーテクスチャ生成
        hr = CreateDummyTexture(device, outSRV, outDesc);
        if (FAILED(hr)) return false;
    }

    return true;
}

void ResourceManager::HDR_CheckHeader(FILE* fp, int* width, int* height)
{
    const int BUF_SIZE = 256;
    char buf[BUF_SIZE];

    // ヘッダチェック
    while (true)
    {
        if (fgets(buf, BUF_SIZE, fp) == NULL)return;
        if (buf[0] == '\n' || buf[0] == '\r') break;

        if (strncmp(buf, "#?", 2) == 0)
        {
            //if (strncmp(buf, "#?RADIANCE", 10) != 0) return -1;
        }
        //	フォーマットチェック
        if (strncmp(buf, "FORMAT=", 7) == 0)
        {
            //if( strncmp( buf, "FORMAT=32-bit_rle_rgbe", 22) != 0 ) return -1;
        }
    }

    // 画像サイズ取得
    if (fgets(buf, BUF_SIZE, fp) == NULL) return;
    char X[5], Y[5];
    sscanf_s(buf, "%s %d %s %d", Y, sizeof(Y), height, X, sizeof(X), width);
}

bool ResourceManager::HDR_ReadLine(BYTE* scanline, FILE* fp, int width)
{
    //	ライン情報読み込み
    int val0 = getc(fp);	// 0x02
    int val1 = getc(fp);	// 0x02
    int val2 = getc(fp);	// size
    int val3 = getc(fp);	// size
    // 幅チェック
    if ((val2 << 8 | val3) != width) return false;

    for (int ch = 0; ch < 4; ch++)
    {
        for (int x = 0; x < width; )
        {
            int length = getc(fp);
            if (length == EOF) return false;

            bool Runlength = false;
            int val = 0;
            if (length > 128) {
                // ランレングス設定
                Runlength = true;
                length -= 128;
                val = getc(fp);
            }
            //	データ読み込み
            while (length > 0) {
                if (Runlength) scanline[x * 4 + ch] = val;
                else scanline[x * 4 + ch] = getc(fp);
                x++;
                length--;
            }
        }
    }
    return true;
}

bool ResourceManager::HDR_ReadPixels(FILE* fp, float* buf, int width, int height)
{
    BYTE scanin[8192 * 4];
    float* work = buf;

    for (int y = height - 1; y >= 0; y--) {
        if (!HDR_ReadLine(scanin, fp, width)) return false;
        for (int x = 0; x < width; x++) {
            float exp = powf(2, scanin[x * 4 + 3] - 128.0f);
            work[0] = scanin[x * 4 + 0] * exp;
            work[1] = scanin[x * 4 + 1] * exp;
            work[2] = scanin[x * 4 + 2] * exp;
            work[3] = 1;
            work += 4;
        }
    }
    return true;
}

bool ResourceManager::LoadHDRTexture(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** outSRV, D3D11_TEXTURE2D_DESC* outDesc)
{
    FILE* fp = nullptr;
    fopen_s(&fp, ConvertString::ConvertToStirng(filename).c_str(), "rb");
    if (fp == NULL) return false;

    // ヘッダーを調べる
    int width = 0.0f, height = 0.0f;
    HDR_CheckHeader(fp, &width, &height);

    // コンバートする
    std::unique_ptr<float[]> buf = std::make_unique<float[]>(width * height * 4);
    HDR_ReadPixels(fp, buf.get(), width, height);
    fclose(fp);

    DirectX::Image img;
    img.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    img.width = width;
    img.height = height;
    img.rowPitch = sizeof(float) * 4 * img.width;
    img.slicePitch = img.rowPitch * img.height;
    img.pixels = (uint8_t*)buf.get();

    DirectX::ScratchImage image;
    image.InitializeFromImage(img);

    DirectX::ScratchImage mipChain;
    DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

    // 画像からシェーダリソースView
    CreateShaderResourceView(device, mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), outSRV);

    // desc取得
    ID3D11Resource* resource;
    ID3D11Texture2D* tex2d;
    (*outSRV)->GetResource(&resource);
    resource->QueryInterface(&tex2d);
    tex2d->GetDesc(outDesc);

    if (resource) resource->Release();
    if (tex2d) tex2d->Release();

    return true;
}