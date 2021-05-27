#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXmath.h>
#include <memory>

#include "Vector.h"

class Texture;
class Shader;

class Skybox
{
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMFLOAT4X4 invView;
		DirectX::XMFLOAT4X4 invProj;
	};

	std::unique_ptr<Texture> mTexture;
	std::unique_ptr<Shader> mShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer;

	Vector3 mEyePos;

	// èâä˙âªä÷êîåQ
	bool CreateShaders();
	bool CreateConstantBuffer();
	bool Initialize();

public:
	Skybox(const wchar_t* filename);
	~Skybox();
	
	void SetEyePos(Vector3 eye);
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj);
};