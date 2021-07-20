#include "Light.h"

#include "lib/Framework.h"

void Light::CreateConstBuffer()
{
	// 定数バッファ
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBForLight);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	FRAMEWORK.GetDevice()->CreateBuffer(&bd, NULL, mConstantBuffer.GetAddressOf());
}

void Light::UpdateConstBuffer()
{
	CBForLight cb;
	cb.lightDir = mLightDir;
	cb.lightColor = mLightColor;

	FRAMEWORK.GetContext()->UpdateSubresource(mConstantBuffer.Get(), 0, NULL, &cb, 0, 0);
}

void Light::SetConstBuffer(int cbSlot)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	context->VSSetConstantBuffers(cbSlot, 1, mConstantBuffer.GetAddressOf());
	context->PSSetConstantBuffers(cbSlot, 1, mConstantBuffer.GetAddressOf());
}
