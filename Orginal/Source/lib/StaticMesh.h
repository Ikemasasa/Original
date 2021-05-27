#pragma once
#include <D3D11.h>
#include <vector>
#include <Directxmath.h>
#include "ResourceManager.h"
#include <string>

/*

現在は使っていない

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
		DirectX::XMFLOAT4X4 worldViewProjection; // ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4 world;				 // ワールド変換行列
		DirectX::XMFLOAT4 lightDirection;		// ライト進行方向
		DirectX::XMFLOAT4 color;			// マテリアルの色

	};

	struct CbufferMaterial
	{
	};


	std::vector<Vertex> vertices;
	std::vector<u_int> indices;
	std::vector<Subset> subsets;
	std::vector<Material> materials;
	//std::wstring texFilename; // png等のファイル名
	std::wstring mtlFilename; // mtlファイル名

	bool LoadOBJFile(const wchar_t* filename); // objファイル読み込み
	bool LoadMTLFile(const wchar_t* filename); // mtlファイル読み込み
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
		ID3D11DeviceContext* context,		 // デバイスコンテキスト
		const DirectX::XMFLOAT4X4& wvp, // ワールド・ビュー・プロジェクション合成行列
		const DirectX::XMFLOAT4X4& world, // ワールド変換行列
		const DirectX::XMFLOAT4& eyePosition,	 // カメラの位置
		const DirectX::XMFLOAT4& color,    // 材質色
		bool isSolid						 // 線・塗りつぶしフラグ
	);

};