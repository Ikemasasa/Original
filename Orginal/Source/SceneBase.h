#pragma once
#include <memory>

#include "lib/Sprite.h"
#include "lib/RenderTarget.h"

#include "CameraBase.h"
#include "ShadowMap.h"

class SceneBase
{
protected:
	Sprite mRamp;
	RenderTarget mSceneTarget;
	ShadowMap mShadowMap;
	std::unique_ptr<Shader> mPostEffect;

public:
	SceneBase() = default;
	virtual ~SceneBase() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};