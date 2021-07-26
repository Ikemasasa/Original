#pragma once
#include <d3d11.h>
#include <wrl.h>

class ConstantBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer;

public:
	void Create(UINT byteWidth);
	void Update(void* data);
	void Set(UINT cbSlot);
};