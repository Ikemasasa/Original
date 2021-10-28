#include "CameraManager.h"

#include "BattleCharacterManager.h"
#include "CameraBase.h"

void CameraManager::Initialize(const Character* target)
{
	mMainCamera.top().get()->Initialize(target);
}

void CameraManager::Update(const Character* target)
{
	mMainCamera.top().get()->Update(target);
}

void CameraManager::Update(const BattleCharacter* target, const BattleCharacterManager* bcm)
{
	mMainCamera.top().get()->Update(target, bcm);
}

void CameraManager::Shake(float duration, float strength)
{
	mMainCamera.top().get()->Shake(duration, strength);
}
