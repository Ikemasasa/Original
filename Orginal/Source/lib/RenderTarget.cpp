#include "RenderTarget.h"

#include "Framework.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Window.h"

void RenderTarget::Initialize(float width, float height)
{
	ID3D11Device* device = FRAMEWORK.GetDevice();

	if (width == 0.0f || height == 0.0f)
	{
		width = Window::GetInstance().GetWidth();
		height = Window::GetInstance().GetHeight();
	}

	{
		// �����_�[�^�[�Q�b�g�ݒ�
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
		td.Width = static_cast<UINT>(width);
		td.Height = static_cast<UINT>(height);
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		// �e�N�X�`������
		HRESULT hr = device->CreateTexture2D(&td, NULL, mRTTexture.GetAddressOf());
		if (FAILED(hr)) return;
		
		//	�����_�[�^�[�Q�b�g�r���[
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		memset(&rtvDesc, 0, sizeof(rtvDesc));
		rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		hr = device->CreateRenderTargetView(mRTTexture.Get(), &rtvDesc, mRTV.GetAddressOf());
	
		// �V�F�[�_���\�[�X�r���[�̐ݒ�
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = rtvDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		// �V�F�[�_���\�[�X�r���[�̐���
		hr = device->CreateShaderResourceView(mRTTexture.Get(), &srvDesc, mSRV.GetAddressOf());
		if (FAILED(hr)) return;
	}

	// �[�x�X�e���V���ݒ�
	{
		// �[�x�X�e���V���ݒ�
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
		td.Width = static_cast<UINT>(width);
		td.Height = static_cast<UINT>(height);
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		// �[�x�X�e���V���e�N�X�`������
		HRESULT hr = device->CreateTexture2D(&td, NULL, mDepth.GetAddressOf());
		if (FAILED(hr)) return;

		// �[�x�X�e���V���r���[�ݒ�
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dsvd.Format = td.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		// �[�x�X�e���V���r���[����
		hr = device->CreateDepthStencilView(mDepth.Get(), &dsvd, mDSV.GetAddressOf());
		if (FAILED(hr)) return;
	}
}

void RenderTarget::Activate(UINT textureSlot)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// �����_�[�^�[�Q�b�g�r���[�ݒ�
	ID3D11RenderTargetView* targets[]={
		mRTV.Get(),
	};

	// �_�~�[SRV���Z�b�g(�G���[�΍�)
	ID3D11ShaderResourceView* dummy = nullptr;
	context->PSSetShaderResources(textureSlot, 1, &dummy);

	context->OMSetRenderTargets( 1, targets, mDSV.Get());
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	if(mRTV.Get())context->ClearRenderTargetView(mRTV.Get(), clearColor);
	if(mDSV.Get())context->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderTarget::Deactivate(UINT textureSlot)
{
	FRAMEWORK.SetRenderTarget();
	FRAMEWORK.GetContext()->PSSetShaderResources(textureSlot, 1, mSRV.GetAddressOf());
}

void RenderTarget::Render(Shader* shader)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	{
		// �`��
		UINT num = 1;
		D3D11_VIEWPORT vp;
		context->RSGetViewports(&num, &vp);
		
		Vector2 pos(0.0f, 0.0f);
		Vector2 scale(1.0f, 1.0f);
		Vector2 texPos(0.0f, 0.0f);
		Vector2 size(vp.Width, vp.Height);
		Renderer2D::GetInstance().Render(mSRV.Get(), shader, pos, scale, texPos, size);
	}
}

void RenderTarget::SetTexture(UINT slot)
{
	FRAMEWORK.GetContext()->PSSetShaderResources(slot, 1, mSRV.GetAddressOf());
}

void RenderTarget::CreateRTV(D3D11_TEXTURE2D_DESC* td, D3D11_RENDER_TARGET_VIEW_DESC* rtvd)
{
	ID3D11Device* device = FRAMEWORK.GetDevice();

	// �����_�[�^�[�Q�b�g�ݒ�
	{
		// �e�N�X�`������
		device->CreateTexture2D(td, NULL, mRTTexture.GetAddressOf());

		//	�����_�[�^�[�Q�b�g�r���[
		device->CreateRenderTargetView(mRTTexture.Get(), rtvd, mRTV.GetAddressOf());
	}
}

void RenderTarget::CreateDSV(D3D11_TEXTURE2D_DESC* td, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvd)
{
	HRESULT hr = S_OK;

	ID3D11Device* device = FRAMEWORK.GetDevice();

	// �[�x�X�e���V���ݒ�
	{
		// �[�x�X�e���V���e�N�X�`������
		device->CreateTexture2D(td, NULL, mDepth.GetAddressOf());

		// �[�x�X�e���V���r���[����
		device->CreateDepthStencilView(mDepth.Get(), dsvd, mDSV.GetAddressOf());
	}
}

void RenderTarget::CreateSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvd, bool isDepth)
{
	ID3D11Device* device = FRAMEWORK.GetDevice();

	ID3D11Resource* tex = nullptr;
	if (!isDepth) tex = mRTTexture.Get();		// �����_�[�^�[�Q�b�g�e�N�X�`��
	else		  tex = mDepth.Get();			// �[�x�X�e���V���e�N�X�`��

	// SRV�쐬
	device->CreateShaderResourceView(tex, srvd, mSRV.GetAddressOf());

}