#include "Primitive.h"

void Primitive::UpdateWorld()
{
	// �e��X�V�̌�ɌĂяo��
	{
		// ���t���[���̃��[���h�s����v�Z
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

		world = DirectX::XMMatrixIdentity();

		S = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);

		Rx = DirectX::XMMatrixRotationX(mAngle.x);	//	X������Ƃ�����]�s��
		Ry = DirectX::XMMatrixRotationY(mAngle.y);	//	Y������Ƃ�����]�s��
		Rz = DirectX::XMMatrixRotationZ(mAngle.z);	//	Z������Ƃ�����]�s��
		R = Rz * Ry * Rx;

		T = DirectX::XMMatrixTranslation(mPos.x, mPos.y, mPos.z);

		world = S * R * T;

		// ���t���[���̃��[���h�s�����
		DirectX::XMStoreFloat4x4(&mWorld, world);
	}
}

void Primitive::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir, const DirectX::XMFLOAT4& color)
{
	DirectX::XMFLOAT4X4 wvp;
	DirectX::XMStoreFloat4x4(&wvp, DirectX::XMLoadFloat4x4(&mWorld) * DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&proj));
	mGP->Render(wvp, mWorld, lightDir, color);
}
