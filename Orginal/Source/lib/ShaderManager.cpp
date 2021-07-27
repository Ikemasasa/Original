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

    // �G���[�o��
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
    // �Ȃ񂩓����Ă���fales
    if (*outVS != NULL) return false;
    if (*outIL != NULL) return false;


    // ���łɓǂݍ��݂��Ă��邩�`�F�b�N
    ResourceVertexShader* find = nullptr;
    for (auto& vs : mVertexShaders)
    {
        if (wcscmp(vs.path.c_str(), filename) != 0) continue;
        if (strcmp(vs.func.c_str(), func) != 0) continue;

        // ������
        find = &vs;
        break;
    }

    // �Ȃ�������
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceVertexShader push;

        // ���_�V�F�[�_
        ID3DBlob* blob = nullptr;
        hr = Compile(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &push.vs);
        if (FAILED(hr))
        {
            blob->Release();
            return false;
        }

        // ���̓��C�A�E�g
        hr = device->CreateInputLayout(
            inputElementDesc,
            numElements,
            blob->GetBufferPointer(),
            blob->GetBufferSize(),
            &push.layout
        );

        blob->Release();
        if (FAILED(hr)) return false;

        // �t�@�C���p�X�A�֐����ۑ�
        push.path = filename;
        push.func = func;

        mVertexShaders.emplace_back(push);
        find = &mVertexShaders.back();
    }

    // ���
    *outVS = find->vs;
    *outIL = find->layout;
    ++find->refNum;

    return true;
}

bool ShaderManager::LoadPixelShader(LPCWSTR filename, LPCSTR func, ID3D11PixelShader** outPS, LPCSTR shadermodel)
{
    if (*outPS != NULL) return false;


    // �T�� 
    ResourceShader<ID3D11PixelShader*>* find;
    for (auto& ps : mPixelShaders)
    {
        if (wcscmp(ps.path.c_str(), filename) != 0) continue;
        if (strcmp(ps.func.c_str(), func) != 0) continue;

        find = &ps;
        break;
    }

    // ������Ȃ�������
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceShader<ID3D11PixelShader*> push;

        // �s�N�Z���V�F�[�_
        ID3DBlob* blob = nullptr;
        hr = Compile(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &push.shader);
        blob->Release();
        if (FAILED(hr)) return false;

        // �t�@�C���p�X�A�֐����ۑ�
        push.path = filename;
        push.func = func;

        mPixelShaders.emplace_back(push);
        find = &mPixelShaders.back();
    }

    // ���
    *outPS = find->shader;
    ++find->refNum;

    return true;

}

bool ShaderManager::LoadGeometryShader(LPCWSTR filename, LPCSTR func, ID3D11GeometryShader** outGS, LPCSTR shadermodel)
{
    if (*outGS != NULL) return false;


    // �T�� 
    ResourceShader<ID3D11GeometryShader*>* find;
    for (auto& gs : mGeometryShaders)
    {
        if (wcscmp(gs.path.c_str(), filename) != 0) continue;
        if (strcmp(gs.func.c_str(), func) != 0) continue;

        find = &gs;
        break;
    }

    // ������Ȃ�������
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceShader<ID3D11GeometryShader*> push;

        // �s�N�Z���V�F�[�_
        ID3DBlob* blob = nullptr;
        hr = Compile(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &push.shader);
        blob->Release();
        if (FAILED(hr)) return false;

        // �t�@�C���p�X�A�֐����ۑ�
        push.path = filename;
        push.func = func;

        mGeometryShaders.emplace_back(push);
        find = &mGeometryShaders.back();
    }

    // ���
    *outGS = find->shader;
    ++find->refNum;

    return true;
}

bool ShaderManager::LoadComputeShader(LPCWSTR filename, LPCSTR func, ID3D11ComputeShader** outCS, LPCSTR shadermodel)
{
    if (*outCS != NULL) return false;


    // �T�� 
    ResourceShader<ID3D11ComputeShader*>* find;
    for (auto& cs : mComputeShaders)
    {
        if (wcscmp(cs.path.c_str(), filename) != 0) continue;
        if (strcmp(cs.func.c_str(), func) != 0) continue;

        find = &cs;
        break;
    }

    // ������Ȃ�������
    if (!find)
    {
        HRESULT hr = S_OK;
        ID3D11Device* device = FRAMEWORK.GetDevice();

        ResourceShader<ID3D11ComputeShader*> push;

        // �s�N�Z���V�F�[�_
        ID3DBlob* blob = nullptr;
        hr = Compile(filename, func, shadermodel, &blob);
        if (FAILED(hr)) return false;

        hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &push.shader);
        blob->Release();
        if (FAILED(hr)) return false;

        // �t�@�C���p�X�A�֐����ۑ�
        push.path = filename;
        push.func = func;

        mComputeShaders.emplace_back(push);
        find = &mComputeShaders.back();
    }

    // ���
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
            // ���X�g�������
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
            // ���X�g�������
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
            // ���X�g�������
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
            // ���X�g�������
            mComputeShaders.erase(it);
        }

        break;
    }
}
