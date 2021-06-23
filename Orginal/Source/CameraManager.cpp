#include "CameraManager.h"

#include "CameraBase.h"

void CameraManager::Update(const Actor* target)
{
	mMainCamera.top().get()->Update(target);
}