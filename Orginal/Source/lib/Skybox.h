#pragma once
#include <DirectXmath.h>
#include <memory>

#include "ConstantBuffer.h"
#include "Vector.h"

class Sprite;
class Shader;

class Skybox
{
	struct CbSky
	{
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMFLOAT4X4 invView;
		DirectX::XMFLOAT4X4 invProj;
	};

	std::unique_ptr<Sprite> mTexture;
	std::unique_ptr<Shader> mShader;
	ConstantBuffer mConstBuffer;

	Vector3 mEyePos;
public:
	Skybox() = default;
	~Skybox() = default;
	
	void Initialize(const wchar_t* filename);
	void Release();
	void SetEyePos(Vector3 eye);
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj);
};