#pragma once
#include <d3d11.h>

class Shader
{
	ID3D11VertexShader*   mVertexShader   = nullptr;
	ID3D11PixelShader*    mPixelShader    = nullptr;
	ID3D11GeometryShader* mGeometryShader = nullptr;
	ID3D11InputLayout*    mVertexLayout   = nullptr;

public:
	Shader() = default;
	~Shader() = default;

	bool Load(LPCWSTR filename, LPCSTR VSFunc, LPCSTR PSFunc, D3D11_INPUT_ELEMENT_DESC* layout = nullptr, UINT numElements = 0);
	bool Load2D(LPCWSTR filename, LPCSTR VSFunc, LPCSTR PSFunc);

	bool LoadGS(LPCWSTR filename, LPCSTR GSFunc);

	void UnLoad();
	void Activate() const;
	void Deactivate() const;
};