#pragma once
#include <memory>
#include "lib/Texture.h"

// class Sprite

class Fade
{ 
public:
	static constexpr float SPEED_FAST = 0.1f;
	static constexpr float SPEED_SLOW = 0.05f;

private:
	enum FadeType
	{
		NONE,
		FADE_IN_OUT,
		FADE_OUT,
		MAX
	};

	float mAlpha = 0.0f;
	float mSpeed = 0.0f;
	std::unique_ptr<Texture> mFade = nullptr;
	FadeType mType = NONE;

	Fade() = default;
	~Fade() = default;

	void UpdateFadeInOut();
	void UpdateFadeOut();


public:
	bool Set(float fadeSpeed);
	bool SetSceneImage(float fadeSpeed);
	bool IsFadeOutEnd() { return mAlpha >= 1.0f; }
	void Update();
	void Render();

public:
	static Fade& GetInstance()
	{
		static Fade instance;
		return instance;
	}
};
