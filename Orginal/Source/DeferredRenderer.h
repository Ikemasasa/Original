#pragma once
#include <memory>
#include <vector>

#include "GBuffer.h"
#include "Light.h"

class ConstantBuffer;
class Shader;

class DeferredRenderer
{
	static const int CB_LIGHT_SLOT = 1;
	static const int POINT_LIGHT_MAX = 16;
	static const int SPOT_LIGHT_MAX = 4;
public:
	struct DeferredPerFrame
	{
		Vector4 eyePos;
	};

	struct DirLight
	{
		Vector4 dir;
		Vector4 color;
	};

	struct PointLight
	{
		Vector4 pos; // xyz:pos, w:range
		Vector4 color;
	};

	struct SpotLightArgu
	{
		Vector3 pos;
		Vector3 target;
		Vector3 rgb;
		float inner; // degree
		float outer; // degree
	};

private:
	struct CBForPointLight
	{
		PointLight pointLight[POINT_LIGHT_MAX];
	};

	struct SpotLight
	{
		Vector4 pos;   // xyz:���W, w:range
		Vector4 dir;   // xyz:����, w:inner_corn
		Vector4 color; // xyz:�F,   w:outer_corn
	};
	struct CBForSpotLight
	{
		SpotLight spotLight[SPOT_LIGHT_MAX];
	};

private:
	std::unique_ptr<GBuffer> mGBuffer;
	std::unique_ptr<Shader> mGBufferShader;
	std::unique_ptr<Shader> mDirLightShader;
	std::unique_ptr<Shader> mPointLightShader;
	std::unique_ptr<Shader> mSpotLightShader;
	std::unique_ptr<ConstantBuffer> mCBPerFrame;
	std::unique_ptr<ConstantBuffer> mCBDirLight;
	std::unique_ptr<ConstantBuffer> mCBPointLight;
	std::unique_ptr<ConstantBuffer> mCBSpotLight;

	std::vector<DirLight> mDirLights;
	std::vector<PointLight> mPointLights;
	std::vector<SpotLight> mSpotLights;

public:
	DeferredRenderer() = default;
	~DeferredRenderer() = default;

	void Initialize(const Vector2 gbufferSize);
	void ActivateGBuffer();
	void DeactivateGBuffer();
	Shader* GetGBufferShader() const { return mGBufferShader.get(); }
	void SetGBufferTexture(UINT startSlot = 0);

	/// <summary>
	/// ���t���[���X�V����萔�o�b�t�@���X�V����֐�
	/// </summary>
	/// <param name="eyePos">�J�������W</param>
	void SetCBPerFrame(const Vector3& eyePos);

	void SetDirLight(std::vector<DirLight>& dirLights);
	void SetPointLight(std::vector<PointLight>& pointLights);
	void SetSpotLight(std::vector<SpotLightArgu>& spotLights);
	void ClearLight();

	void Render();
};