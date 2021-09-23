#pragma once
#include <DirectXmath.h>
#include <memory>

#include "ConstantBuffer.h"
#include "Matrix.h"
#include "Vector.h"

class Sprite;
class Shader;

class Skybox
{
	struct CbSky
	{
		Vector4 cameraPos;
		Matrix invView;
		Matrix invProj;
	};

	std::unique_ptr<Sprite> mTexture;
	std::unique_ptr<Shader> mShader;
	ConstantBuffer mConstBuffer;

	Vector3 mEyePos = {};
	UINT mTextureSlot = -1;

public:
	Skybox() = default;
	~Skybox() = default;
	
	void Initialize(const wchar_t* filename);
	void Release();
	void SetEyePos(Vector3 eye);
	void Render(const Matrix& view, const Matrix& proj);
	
	void SetEnvTexture(UINT slot);
};