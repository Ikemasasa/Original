#pragma once
#include <memory>

#include "lib/RenderTarget.h"

class Shader;

class GBuffer
{
public:
	enum Kind { COLOR, POS, NORMAL, NUM };

private:
	std::unique_ptr<Shader> mShader;
	RenderTarget mTargets[NUM];
	Vector2 mTargetSize;
	UINT mSlot = 0;

public:
	GBuffer() = default;
	virtual ~GBuffer() = default;

	void Initialize(float width, float height);
	void Activate();
	void Deactivate();

	void SetTexture(UINT startSlot);
	
	RenderTarget* GetColorTarget() { return &mTargets[COLOR]; }
	Vector2 GetTargetSize() const { return mTargetSize; }
};