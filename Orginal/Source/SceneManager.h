#pragma once
#include <windows.h>
#include <stack>
#include <memory>

class SceneBase;

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

	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}
};