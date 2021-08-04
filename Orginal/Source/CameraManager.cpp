#include "CameraManager.h"

#include "CameraBase.h"

void CameraManager::Update(const Character* target)
{
	mMainCamera.top().get()->Update(target);
}