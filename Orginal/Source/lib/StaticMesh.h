#pragma once
#include <D3D11.h>
#include <vector>
#include <Directxmath.h>
#include "ResourceManager.h"
#include <string>

/*

���݂͎g���Ă��Ȃ�

*/



class StaticMesh
{
private:
	ID3D11VertexShader* pVertexShader   = nullptr;
	ID3D11PixelShader* pPixelShader		= nullptr;
	ID3D11InputLayout* pInputLayout     = nullptr;
	ID3D11Buffer* pVertexBuffer         = nullptr;
	ID3D11Buffer* pIndexBuffer		    = nullptr;
	ID3D11Buffer* pConstBufferScene		= nullptr;
	ID3D11Buffer* pConstBufferMaterial  = nullptr;
	ID3D11RasterizerState* pWireframeRS = nullptr;
	ID3D11RasterizerState* pSolidRS		= nullptr;
	ID3D11DepthStencilState* pDsState	= nullptr;
	ID3D11SamplerState* pSamplerState	= nullptr;

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};

	struct Material
	{
		std::wstring newmtl;
		DirectX::XMFLOAT3 Ka = { 0.2f, 0.2f, 0.2f };
		DirectX::XMFLOAT3 Kd = { 0.8f, 0.8f, 0.8f };
		DirectX::XMFLOAT3 Ks = { 1.0f, 1.0f, 1.0f };
		u_int illum = 1;
		std::wstring map_Kd;
		ID3D11ShaderResourceView* pShaderResourceView = nullptr;
		D3D11_TEXTURE2D_DESC tex2DDesc = {};
	};

	struct Subset
	{
		std::wstring usemtl;
		u_int indexStart = 0;
		u_int indexCount = 0;
	};

	struct CbufferScene
	{
		DirectX::XMFLOAT4X4 worldViewProjection; // ���[���h�E�r���[�E�v���W�F�N�V���������s��
		DirectX::XMFLOAT4X4 world;				 // ���[���h�ϊ��s��
		DirectX::XMFLOAT4 lightDirection;		// ���C�g�i�s����
		DirectX::XMFLOAT4 color;			// �}�e���A���̐F

	};

	struct CbufferMaterial
	{
	};


	std::vector<Vertex> vertices;
	std::vector<u_int> indices;
	std::vector<Subset> subsets;
	std::vector<Material> materials;
	//std::wstring texFilename; // png���̃t�@�C����
	std::wstring mtlFilename; // mtl�t�@�C����

	bool LoadOBJFile(const wchar_t* filename); // obj�t�@�C���ǂݍ���
	bool LoadMTLFile(const wchar_t* filename); // mtl�t�@�C���ǂݍ���
	void Initialize(ID3D11Device* device, const wchar_t* filename);

public:
	StaticMesh(ID3D11Device* device, const wchar_t* filename)
	{
		vertices.clear();
		indices.clear();
		subsets.clear();
		materials.clear();

		Initialize(device, filename);
	}
	virtual ~StaticMesh()
	{
		ResourceManager::ReleaseVertexShader(pVertexShader, pInputLayout);
		ResourceManager::ReleasePixelShader(pPixelShader);
		for (auto& m : materials) ResourceManager::ReleaseShaderResourceView(m.pShaderResourceView);
		if (pVertexBuffer) pVertexBuffer->Release();
		if (pIndexBuffer) pIndexBuffer->Release();
		if (pConstBufferScene) pConstBufferScene->Release();
		if (pConstBufferMaterial) pConstBufferMaterial->Release();
		if (pWireframeRS) pWireframeRS->Release();
		if (pSolidRS) pSolidRS->Release();
		if (pDsState) pDsState->Release();
		if (pSamplerState)pSamplerState->Release();
		
		vertices.clear();
		indices.clear();
		materials.clear();
		subsets.clear();
	}


	void Render(
		ID3D11DeviceContext* context,		 // �f�o�C�X�R���e�L�X�g
		const DirectX::XMFLOAT4X4& wvp, // ���[���h�E�r���[�E�v���W�F�N�V���������s��
		const DirectX::XMFLOAT4X4& world, // ���[���h�ϊ��s��
		const DirectX::XMFLOAT4& eyePosition,	 // �J�����̈ʒu
		const DirectX::XMFLOAT4& color,    // �ގ��F
		bool isSolid						 // ���E�h��Ԃ��t���O
	);

};