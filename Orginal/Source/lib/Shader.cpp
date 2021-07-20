#include "Shader.h"

#include "Framework.h"
#include "ResourceManager.h"

bool Shader::Load(LPCWSTR filename, LPCSTR VSFunc, LPCSTR PSFunc, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements)
{
	if (!layout)
	{
		D3D11_INPUT_ELEMENT_DESC elementDesc[] = {
			{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 0   , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 4 * 15, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT elementDescNum = ARRAYSIZE(elementDesc);

		layout = elementDesc;
		numElements = elementDescNum;
	}

	LPCSTR vsShaderModel = "vs_4_0_level_9_3";
	LPCSTR psShaderModel = "ps_4_0_level_9_3";

	bool check = false;
	check = ResourceManager::LoadVertexShader(filename, VSFunc, layout, numElements, &mVertexShader, &mVertexLayout, vsShaderModel);
	if (!check) return false;

	check = ResourceManager::LoadPixelShader(filename, PSFunc, &mPixelShader, psShaderModel);
	if (!check)return false;

	return true;
}

bool Shader::Load2D(LPCWSTR filename, LPCSTR VSFunc, LPCSTR PSFunc)
{
	// 2D用のシェーダ読み込み(結構数がおおくて、layoutを毎回用意するのがめんどくさかったから関数化)
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 0   , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	LPCSTR vsShaderModel = "vs_4_0_level_9_3";
	LPCSTR psShaderModel = "ps_4_0_level_9_3";

	bool check = false;
	check = ResourceManager::LoadVertexShader(filename, VSFunc, layout, numElements, &mVertexShader, &mVertexLayout, vsShaderModel);
	if (!check) return false;

	check = ResourceManager::LoadPixelShader(filename, PSFunc, &mPixelShader, psShaderModel);
	if (!check)return false;

	return true;
}

void Shader::UnLoad()
{
	ResourceManager::ReleaseVertexShader(mVertexShader, mVertexLayout);
	ResourceManager::ReleasePixelShader(mPixelShader);
}

void Shader::Activate() const
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	context->IASetInputLayout(mVertexLayout);
	context->VSSetShader(mVertexShader, NULL, 0);
	context->PSSetShader(mPixelShader, NULL, 0);
}

void Shader::Deactivate() const
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	context->VSSetShader(NULL, NULL, 0);
	context->PSSetShader(NULL, NULL, 0);
}
