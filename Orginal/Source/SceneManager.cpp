#include "SceneManager.h"

#include "SceneBase.h"
#include "SceneLoad.h"

void SceneManager::Initialize(std::unique_ptr<SceneBase> scene)
{
	mRunScene.emplace(scene.release());
	mRunScene.top()->Initialize();
}

void SceneManager::Update()
{
	if (mIsPopCurScene)
	{
		mRunScene.top()->Release();
		mRunScene.pop();
		mIsPopCurScene = false;
	}
	if (mNextScene.get())
	{
		if(!mIsStack) Release();
		
		mRunScene.emplace(mNextScene.release());
		if (!mRunScene.top()->IsLoadFinished())
		{
			mRunScene.top()->Initialize();
		}
	}

	if (mRunScene.empty()) return;
	mRunScene.top()->Update();
}

void SceneManager::Render()
{
	if (mRunScene.empty()) return;
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
	if (mNextScene) return;
	mNextScene.reset(nextScene.release());
	mIsStack = false;
}

void SceneManager::SetStackScene(std::unique_ptr<SceneBase> nextScene)
{
	if (mNextScene) return;
	mNextScene.reset(nextScene.release());
	mIsStack = true;
}

void SceneManager::PopCurrentScene()
{
	mIsPopCurScene = true;
}

