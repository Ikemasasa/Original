#pragma once
#include <memory>

#include "lib/RenderTarget.h"
#include "lib/Shader.h"
#include "lib/Sprite.h"

#include "Bloom.h"
#include "CameraBase.h"
#include "DeferredRenderer.h"
#include "GBuffer.h"
#include "ShadowMap.h"
#include "Light.h"

class SceneBase
{
protected:

	static Light mLight;

	std::unique_ptr<Sprite> mRamp;
	std::unique_ptr<Shader> mPostEffect;
	std::unique_ptr<ShadowMap> mShadowMap;
	std::unique_ptr<RenderTarget> mSceneTarget;
	std::unique_ptr<Bloom> mBloom;

	// ディファード
	std::unique_ptr<DeferredRenderer> mDeferredRenderer;

	// ロード終了フラグ
	bool mIsLoadFinished = false;

protected: // 関数
	void InitializeBaseAll();
	void CreateBaseAll();
	void ReleaseBaseAll();
	virtual void SetDeferredParam() {}

public:
	SceneBase() = default;
	virtual ~SceneBase() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	bool IsLoadFinished() const { return mIsLoadFinished; }
	void LoadFinished() { mIsLoadFinished = true; }
};