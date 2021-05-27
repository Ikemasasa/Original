#pragma once
#include <d3d11.h>
#include <wrl.h>

class ComputeShader
{
	static const int INPUT_BUFFER_MAX = 4;

	ID3D11ComputeShader*							  mCS					 = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  mSRV[INPUT_BUFFER_MAX] = { nullptr };
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV				     = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mStructBuf[INPUT_BUFFER_MAX]	 = {nullptr};
	Microsoft::WRL::ComPtr<ID3D11Buffer> mRWBuf							 = nullptr;
	UINT mStructBufNum = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer = nullptr;

public:
	ComputeShader(LPCWSTR filename, LPCSTR func, LPCSTR shaderModel = "cs_4_0");
	~ComputeShader();

	bool CreateConstantBuffer(UINT elementSize);
	bool CreateStructuredBuffer(void* initData, UINT count, UINT elementSize);
	bool CreateRWStructuredBuffer(UINT count, UINT elementSize);
	bool CreateRWByteaddressBuffer(void* initData, UINT count, UINT elementSize);
	bool CreateResultBuffer(ID3D11Buffer** outBuffer);

	bool UpdateStructureBuffer(void* data, UINT index);
	bool UpdateConstantBuffer(void* data);

	void Run(UINT x, UINT y, UINT z);

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetRWBuffer() { return mRWBuf; }
};