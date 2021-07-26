#pragma once
#include <memory>

#include "lib/Sprite.h"
#include "lib/RenderTarget.h"

#include "CameraBase.h"
#include "Light.h"
#include "ShadowMap.h"

class SceneBase
{
protected:
	enum GBuffer {COLOR, POS, NORMAL, NUM};
	static const int SHADOWMAP_TEXTURE_SLOT = 14;
	static Light mLight;

	Sprite mRamp;
	ShadowMap mShadowMap;
	std::unique_ptr<Shader> mPostEffect;
	RenderTarget mSceneTarget;

	// GBuffer�֘A
	std::unique_ptr<Shader> mGBufferShader;
	RenderTarget mGBufferColor;
	RenderTarget mGBufferPosition;
	RenderTarget mGBufferNormal;

	// �f�B�t�@�[�h
	std::unique_ptr<Shader> mDeferredDirLightShader;

	ConstantBuffer mCBForDeferred;
	struct CBForDeferredPerFrame
	{
		Vector4 eyePos;		//�J�������W
	};

protected: // �֐�
	void CreatePostEffectShader();

	// Gbuffer�֘A
	void InitializeGBuffer();
	void ActivateGBuffer(UINT startSlot = 0);
	void DeactivateGBuffer();
	void SetGBufferTexture(UINT startSlot = 0);


public:
	SceneBase() = default;
	virtual ~SceneBase() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};