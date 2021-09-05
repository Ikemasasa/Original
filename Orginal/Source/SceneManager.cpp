#include "SceneManager.h"
#include "SceneBase.h"

void SceneManager::Initialize(std::unique_ptr<SceneBase> scene)
{
	mRunScene.emplace(scene.release());
	mRunScene.top()->Initialize();
}

void SceneManager::Update()
{
	if (mNextScene.get())
	{
		if(!mIsStack) Release();
		
		mRunScene.emplace(mNextScene.release());
		mRunScene.top()->Initialize();
	}
	if (mIsPopCurScene)
	{
		mRunScene.top()->Release();
		mRunScene.pop();
		mIsPopCurScene = false;
	}

	mRunScene.top()->Update();
}

void SceneManager::Render()
{
	mRunScene.top()->Render();
}

void SceneManager::Release()
{
	// ベクター分全部回す
	size_t size = mRunScene.size();
	for (size_t i = 0; i < size; ++i)
	{
		mRunScene.top()->Release();
		mRunScene.pop();
	}
}

void SceneManager::SetNextScene(std::unique_ptr<SceneBase> nextScene)
{
	if (mNextScene.get()) return;
	mNextScene.reset(nextScene.release());
	mIsStack = false;
}

void SceneManager::SetStackScene(std::unique_ptr<SceneBase> nextScene)
{
	if (mNextScene.get()) return;
	mNextScene.reset(nextScene.release());
	mIsStack = true;
}

void SceneManager::PopCurrentScene()
{
	mIsPopCurScene = true;
}

