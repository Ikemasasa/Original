#pragma once
#include <d3d11.h>

class Shader
{
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11PixelShader*  mPixelShader  = nullptr;
	ID3D11InputLayout*  mVertexLayout = nullptr;

public:
	Shader() = default;
	~Shader() = default;

	bool Load(LPCWSTR filename, LPCSTR VSFunc, LPCSTR PSFunc);
	//bool Load(LPCWSTR filename, LPCSTR VSFunc, LPCSTR PSFunc, LPCSTR vsShaderModel = "vs_5_0", LPCSTR psShaderModel = "ps_5_0");
	void UnLoad();
	void Activate() const;
	void Deactivate() const;
};