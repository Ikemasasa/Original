#pragma once
#include <memory>

#include "lib/ConstantBuffer.h"
#include "lib/Shader.h"


class Sprite;

class Fade
{ 
public:
	static constexpr float SPEED_VERYFAST = 0.2f;
	static constexpr float SPEED_FAST = 0.1f;
	static constexpr float SPEED_SLOW = 0.05f;
	static constexpr float SPEED_VERY_SLOW = 0.025f;


	enum FadeType
	{
		NONE,
		FADE_OUT,
		FADE_IN,
		MAX
	};
private:
	struct CBForWave
	{
		int WaveNum = 0;
		float sinFactor = 0.0f;
		float dummy0 = 0.0f;
		float dummy1 = 0.0f;
	};

	float mAlpha = 0.0f;
	float mSpeed = 0.0f;
	FadeType mType = NONE;
	std::unique_ptr<Sprite> mFade = nullptr;
	
	// ウェーブ用
	static const int WAVENUM = 3;
	static constexpr float ADD_SINFACTOR = 1.5f * 0.1745f; // 1度(radian
	bool mIsWave = false;
	float mSinFactor = 0.0f;
	ConstantBuffer mWaveCB;
	std::unique_ptr<Shader> mWaveShader = nullptr;

	Fade() = default;
	~Fade() = default;

	void UpdateFadeIn();
	void UpdateFadeOut();


public:
	bool Set(float fadeSpeed);
	bool SetSceneImage(float fadeSpeed, bool isWave = false);
	void Update();
	void Render();

	bool IsFadeOutEnd() { return mAlpha >= 1.0f; }

	bool CheckFadeState(Fade::FadeType type, float alpha);
public:
	static Fade& GetInstance()
	{
		static Fade instance;
		return instance;
	}
};
