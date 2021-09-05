#include "CameraManager.h"

#include "CameraBase.h"

void CameraManager::Initialize(const Character* target)
{
	mMainCamera.top().get()->Initialize(target);
}

void CameraManager::Update(const Character* target)
{
	mMainCamera.top().get()->Update(target);
}