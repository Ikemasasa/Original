#include "ComputeShader.h"

#include "Framework.h"
#include "ResourceManager.h"

ComputeShader::ComputeShader(LPCWSTR filename, LPCSTR func, LPCSTR shaderModel)
{
	ResourceManager::LoadComputeShader(filename, func, &mCS, shaderModel);
}

ComputeShader::~ComputeShader()
{
	ResourceManager::ReleaseComputeShader(mCS);
}

bool ComputeShader::CreateConstantBuffer(UINT elementSize)
{
	HRESULT hr = S_OK;
	ID3D11Device* device = FRAMEWORK.GetDevice();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = elementSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&desc, NULL, mConstBuffer.GetAddressOf());
	if (FAILED(hr)) return false;
	
	return true;
}

bool ComputeShader::CreateStructuredBuffer(void* initData, UINT count, UINT elementSize)
{
	if (mStructBufNum >= INPUT_BUFFER_MAX) return false;

	HRESULT hr = S_OK;
	ID3D11Device* device = FRAMEWORK.GetDevice();

	// バッファー
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = elementSize * count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;

	if (initData)
	{
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = initData;
		hr = device->CreateBuffer(&desc, &sd, mStructBuf[mStructBufNum].GetAddressOf());
	}
	else
	{
		hr = device->CreateBuffer(&desc, nullptr, mStructBuf[mStructBufNum].GetAddressOf());
	}

	if (FAILED(hr)) return false;

	// ビュー
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvd.BufferEx.FirstElement = 0;
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
	hr = device->CreateShaderResourceView(mStructBuf[mStructBufNum].Get(), &srvd, mSRV[mStructBufNum].GetAddressOf());
	if (FAILED(hr)) return false;


	mStructBufNum++;

	return true;
}

bool ComputeShader::CreateRWStructuredBuffer(UINT count, UINT elementSize)
{
	if (mRWBuf.Get()) return false;

	HRESULT hr = S_OK;
	ID3D11Device* device = FRAMEWORK.GetDevice();

	// バッファー
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = elementSize * count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;
	hr = device->CreateBuffer(&desc, nullptr, mRWBuf.GetAddressOf());
	if (FAILED(hr)) return false;

	// ビュー
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
	ZeroMemory(&uavd, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavd.Buffer.FirstElement = 0;
	uavd.Format = DXGI_FORMAT_UNKNOWN;
	uavd.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
	hr = device->CreateUnorderedAccessView(mRWBuf.Get(), &uavd, mUAV.GetAddressOf());
	if (FAILED(hr)) return false;

	return true;
}

bool ComputeShader::CreateRWByteaddressBuffer(void* initData, UINT count, UINT elementSize)
{
	if (mRWBuf.Get()) return false;

	HRESULT hr = S_OK;
	ID3D11Device* device = FRAMEWORK.GetDevice();

	// バッファー
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = count * elementSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	if (initData)
	{
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = initData;
		hr = device->CreateBuffer(&desc, &sd, mRWBuf.GetAddressOf());
	}
	else
	{
		hr = device->CreateBuffer(&desc, nullptr, mRWBuf.GetAddressOf());
	}
	if (FAILED(hr))return false;

	// ビュー
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
	ZeroMemory(&uavd, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavd.Format = DXGI_FORMAT_R32_TYPELESS;
	uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavd.Buffer.FirstElement = 0;
	uavd.Buffer.NumElements = count;
	uavd.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	device->CreateUnorderedAccessView(mRWBuf.Get(), &uavd, mUAV.GetAddressOf());
	if (FAILED(hr))return false;

	return true;
}

bool ComputeShader::UpdateStructureBuffer(void* data, UINT index)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();
	context->UpdateSubresource(mStructBuf[index].Get(), 0, NULL, data, 0, 0);

	return true;
}

bool ComputeShader::UpdateConstantBuffer(void* data)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, data, 0, 0);
	return true;
}

bool ComputeShader::CreateResultBuffer(ID3D11Buffer** outBuffer)
{
	// RWStructuredBufferからCPUで直接読み込めないから橋渡しのBufferを作る
	ID3D11Device* device = FRAMEWORK.GetDevice();
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	mRWBuf->GetDesc(&bd);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bd.Usage = D3D11_USAGE_STAGING;
	bd.BindFlags = 0;
	bd.MiscFlags = 0;

	hr = device->CreateBuffer(&bd, nullptr, outBuffer);
	if (FAILED(hr)) return false;

	ID3D11DeviceContext* context = FRAMEWORK.GetContext();
	context->CopyResource(*outBuffer, mRWBuf.Get());

	return true;
}

void ComputeShader::Run(UINT x, UINT y, UINT z)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	context->CSSetShader(mCS, nullptr, 0);
	context->CSSetShaderResources(0, mStructBufNum, mSRV[0].GetAddressOf());
	context->CSSetUnorderedAccessViews(0, 1, mUAV.GetAddressOf(), nullptr);
	context->CSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

	context->Dispatch(x, y, z);
}

