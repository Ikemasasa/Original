#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXmath.h>
#include <memory>

#include "Vector.h"

class Sprite;
class Shader;

class Skybox
{
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMFLOAT4X4 invView;
		DirectX::XMFLOAT4X4 invProj;
	};

	std::unique_ptr<Sprite> mTexture;
	std::unique_ptr<Shader> mShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer;

	Vector3 mEyePos;

	// èâä˙âªä÷êî
	bool CreateShaders();
	bool CreateConstantBuffer();

public:
	Skybox() = default;
	~Skybox() = default;
	
	void Initialize(const wchar_t* filename);
	void Release();
	void SetEyePos(Vector3 eye);
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj);
};