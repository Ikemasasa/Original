#pragma once
#include "SceneBase.h"

#include <stack>
#include <memory>



class SceneManager
{
private:
	std::stack<std::unique_ptr<SceneBase>> mRunScene;
	std::unique_ptr<SceneBase> mNextScene;
	bool mIsStack = false;
	bool mIsPopCurScene = false;

public:
	void Initialize(std::unique_ptr<SceneBase> scene);
	void Update();
	void Render();
	void Release();
	void SetNextScene(std::unique_ptr<SceneBase> nextScene);
	void SetStackScene(std::unique_ptr<SceneBase> nextScene);
	void PopCurrentScene();

	bool IsLoop() const { return (mRunScene.size() > 0) || !mIsPopCurScene; }

	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}
};