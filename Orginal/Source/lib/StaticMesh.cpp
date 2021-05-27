#include "StaticMesh.h"
#include "ResourceManager.h"
#include <fstream>
#include <string>

bool StaticMesh::LoadOBJFile(const wchar_t* filename)
{
	std::wifstream fin(filename);
	if (!fin.is_open()) return false;

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> texcoords;
	u_int indexNum = 0;
	u_int subsetNum = 0;
	bool vReverseFlag = false; // V座標を反転させるかどうか
	bool originChangeFlag = false; // Y座標を - 3するかどうか

	const wchar_t* DCCtools[] = {
		 L"Cheetah3D)",
		 L"Blender"
	};

	enum DCCtoolName
	{
		CHEETAH3D,
		BLENDER
	};

	const wchar_t* mtlName = L"mtllib"; // マテリアルのやつ
	const wchar_t* usemtl = L"usemtl";
	const wchar_t* positionData = L"v"; // 頂点座標
	const wchar_t* texcoordData = L"vt";// テクスチャ座標
	const wchar_t* normalData = L"vn"; // 法線
	const wchar_t* IndexData = L"f"; // 面情報
	std::wstring chunk;

	while(true)
	{
		fin >> chunk;
		if (fin.eof()) break;

		if (chunk == DCCtools[CHEETAH3D])
		{
			vReverseFlag = true;
		}
		else if (chunk == DCCtools[BLENDER])
		{
			originChangeFlag = true;
			vReverseFlag = true;
		}
		else if (chunk == mtlName)
		{
			fin >> chunk;
			mtlFilename += chunk;
		}
		else if (chunk == positionData)
		{
			DirectX::XMFLOAT3 pos = {};
			fin >> pos.x >> pos.y >> pos.z;
			positions.push_back(pos);
		}
		else if (chunk == texcoordData)
		{
			DirectX::XMFLOAT2 texcoord;
			fin >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (chunk == normalData)
		{
			DirectX::XMFLOAT3 nor = {};
			fin >> nor.x >> nor.y >> nor.z;
			normals.push_back(nor);
		}
		else if (chunk == usemtl)
		{
			fin >> chunk;
			Subset s = {};
			s.usemtl = chunk;
			s.indexStart = indexNum;
			subsets.push_back(s);
		}
		else if (chunk == IndexData)
		{
			for (u_int i = 0; i < 3; ++i)
			{
				Vertex vertex;
				u_int v, vt, vn;

				fin >> v;
				vertex.position = positions[v - 1];
				if ('/' == fin.peek())
				{
					fin.ignore();
					if ('/' != fin.peek())
					{
						fin >> vt;
						vertex.texcoord = texcoords[vt - 1];

					}

					if ('/' == fin.peek())
					{
						fin.ignore();
						fin >> vn;
						vertex.normal = normals[vn - 1];
					}
				}

				vertices.push_back(vertex);
				indices.push_back(indexNum++);
				++subsets[subsets.size() - 1].indexCount;
			}

		}

	}


	if (vReverseFlag)
	{
		for (auto& v : vertices)
		{
			//v.texcoord.y = 1.0f - v.texcoord.y;
		}
	}
	if (originChangeFlag)
	{
		for (auto& v : vertices)
		{
			//v.position.y -= 3.0f;
		}
	}

	fin.close();
	return true;
}

bool StaticMesh::LoadMTLFile(const wchar_t* filename)
{
	std::wifstream fin(filename);
	if (!fin.is_open()) return false;

	const wchar_t* newmtl = L"newmtl";
	const wchar_t* ambient = L"Ka"; // ambient[環境光] 3つの 0.0 ~ 1.0 までの数値で決まる 
	const wchar_t* diffuse = L"Kd"; // diffuse[拡散光] 3つの 0.0 ~ 1.0 までの数値で決まる
	const wchar_t* specular = L"Ks"; // specular[鏡面光] 3つの0.0 ~ 1.0 までの数値で決まる
	const wchar_t* shininess = L"Ns"; // shininess[鏡面率] (1つの)0.0 ~ 1000.0 までの数値で決まる
	const wchar_t* illum = L"illum";
	const wchar_t* useTexture = L"map_Kd"; // テクスチャで使ってる画像の名前?(よくわかってない)

	std::wstring chunk;
	Material m;
	while (fin)
	{
		fin >> chunk;
		if (fin.eof()) break;
		if (chunk == newmtl)
		{
			if (!m.newmtl.empty())
			{
				materials.push_back(m);
				m = {};
			}
			fin >> m.newmtl;
		}
		else if (chunk == ambient)
		{
			fin >> m.Ka.x >> m.Ka.y >> m.Ka.z;
		}
		else if (chunk == diffuse)
		{
			fin >> m.Kd.x >> m.Kd.y >> m.Kd.z;
		}
		else if (chunk == specular)
		{
			fin >> m.Ks.x >> m.Ks.y >> m.Ks.z;
		}
		else if (chunk == illum)
		{
			fin >> m.illum;
		}
		else if (chunk == useTexture)
		{
			fin >> m.map_Kd;
		}
	}

	materials.push_back(m);

	return true;
}

void StaticMesh::Initialize(ID3D11Device* device, const wchar_t* filename)
{
	HRESULT hr = S_OK;

	std::wstring fileAddress = filename;
	while (*fileAddress.rbegin() != L'/') fileAddress.pop_back(); // あとで ～.png 等を足すため
	mtlFilename = fileAddress + mtlFilename;

	if (!LoadOBJFile(filename)) return;
	if (!LoadMTLFile(mtlFilename.c_str()))return;
	for (auto& it : materials) if(!it.map_Kd.empty()) it.map_Kd = fileAddress + it.map_Kd;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		 0, sizeof(float) * 0,
		 D3D11_INPUT_PER_VERTEX_DATA, 0},

		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		 0, sizeof(float) * 3,
		 D3D11_INPUT_PER_VERTEX_DATA, 0},
	   
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		  0, sizeof(float) * (3 + 3),
		  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	ResourceManager::LoadVertexShader(device, "./Source/Shaders/StaticMesh/StaticMesh_vs.cso", layout, numElements, &pVertexShader, &pInputLayout);
	ResourceManager::LoadPixelShader(device, "./Source/Shaders/StaticMesh/StaticMesh_ps.cso", &pPixelShader);

	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.FrontCounterClockwise = false;
	hr = device->CreateRasterizerState(&rsDesc, &pWireframeRS);
	if (FAILED(hr)) return;

	rsDesc.FillMode = D3D11_FILL_SOLID;
	hr = device->CreateRasterizerState(&rsDesc, &pSolidRS);
	if (FAILED(hr)) return;

	for (auto& it : materials)
	{
		ResourceManager::LoadShaderResource(device, it.map_Kd.c_str(), &it.pShaderResourceView, &it.tex2DDesc);
	}

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&samplerDesc, &pSamplerState);
	if (FAILED(hr))return;

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.StencilEnable = false;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = device->CreateDepthStencilState(&dsDesc, &pDsState);
	if (FAILED(hr)) return;


	D3D11_SUBRESOURCE_DATA subresourceData;
	ZeroMemory(&subresourceData, sizeof(subresourceData));
	subresourceData.pSysMem = vertices.data();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bufferDesc, &subresourceData, &pVertexBuffer);
	if (FAILED(hr)) return;


	subresourceData.pSysMem = indices.data();
	bufferDesc.ByteWidth = sizeof(u_int) * indices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hr = device->CreateBuffer(&bufferDesc, &subresourceData, &pIndexBuffer);
	if (FAILED(hr)) return;

	bufferDesc.ByteWidth = sizeof(CbufferScene);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bufferDesc, NULL, &pConstBufferScene);
	if (FAILED(hr)) return;

	bufferDesc.ByteWidth = sizeof(CbufferMaterial);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bufferDesc, NULL, &pConstBufferMaterial);
	if (FAILED(hr)) return;
}

void StaticMesh::Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& wvp, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& eyePosition, const DirectX::XMFLOAT4& color, bool isSolid)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetInputLayout(pInputLayout);
	context->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (isSolid)	context->RSSetState(pSolidRS);
	else		context->RSSetState(pWireframeRS);


	context->VSSetShader(pVertexShader, NULL, 0);
	context->PSSetShader(pPixelShader, NULL, 0);
	context->OMSetDepthStencilState(pDsState, 0);

	for (auto& material : materials)
	{
		CbufferScene cbs;
		cbs.worldViewProjection = wvp;
		cbs.world = world;
		cbs.lightDirection = { 0.0f, -1.0f, 1.0f, 1.0f };
		cbs.color.x = color.x * material.Kd.x;
		cbs.color.y = color.y * material.Kd.y;
		cbs.color.z = color.z * material.Kd.z;
		cbs.color.w = color.w;

		context->UpdateSubresource(pConstBufferScene, 0, NULL, &cbs, 0, 0);
		context->VSSetConstantBuffers(0, 1, &pConstBufferScene);

		context->PSSetShaderResources(0, 1, &material.pShaderResourceView);
		context->PSSetSamplers(0, 1, &pSamplerState);

		for (auto& subset : subsets)
		{
			if(material.newmtl == subset.usemtl)
				context->DrawIndexed(subset.indexCount, subset.indexStart, 0);
		}
	}
}