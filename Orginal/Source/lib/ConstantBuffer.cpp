#include "ConstantBuffer.h"

#include "Framework.h"

void ConstantBuffer::Create(UINT byteWidth)
{
	if (mConstBuffer) return;

	// 定数バッファ
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	FRAMEWORK.GetDevice()->CreateBuffer(&bd, NULL, mConstBuffer.GetAddressOf());
}

void ConstantBuffer::Update(void* data)
{
	FRAMEWORK.GetContext()->UpdateSubresource(mConstBuffer.Get(), 0, NULL, data, 0, 0);
}

void ConstantBuffer::Set(UINT cbSlot)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	context->VSSetConstantBuffers(cbSlot, 1, mConstBuffer.GetAddressOf());
	context->PSSetConstantBuffers(cbSlot, 1, mConstBuffer.GetAddressOf());
}
