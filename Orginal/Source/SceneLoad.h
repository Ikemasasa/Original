#pragma once
#include <memory>

#include "SceneBase.h"

class Sprite;

class SceneLoad : public SceneBase
{
	static constexpr float SIN_MUL = 5.0f;
	static constexpr float TEXT_X = 900.0f;
	static constexpr float TEXT_Y = 600.0f;

	
	std::unique_ptr<SceneBase> mLoadScene;
	std::unique_ptr<Sprite> mStr;
	std::unique_ptr<Sprite> mBG;
	float mSinAngle = 0.0f;


public:
	SceneLoad(std::unique_ptr<SceneBase> loadScene);
	~SceneLoad() = default;
	

	void Initialize();
	void Update();
	void Render();
	void Release();
};