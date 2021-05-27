#include "CameraManager.h"

#include "CameraBase.h"

void CameraManager::Update(const std::shared_ptr<Actor>& target)
{
	mMainCamera.top().get()->Update(target);
}