#include "CameraManager.h"

#include "CameraBase.h"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::Initialize(const Vector3& target, float distance)
{
	mMainCamera.top().get()->Initialize(target, distance);
}

void CameraManager::Update(const Vector3& target)
{
	mMainCamera.top().get()->Update(target);
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

 Matrix CameraManager::GetView() const
{
	return mMainCamera.top().get()->GetViewMatrix();
}

 Matrix CameraManager::GetProj() const
{
	return mMainCamera.top().get()->GetProjectionMatrix();
}

 Vector3 CameraManager::GetPos() const
{
	return mMainCamera.top().get()->GetPos();
}

 float CameraManager::GetZenithAngle() const
{
	return mMainCamera.top().get()->GetZenithAngle();
}

 float CameraManager::GetAzimuthAngle() const
{
	return  mMainCamera.top().get()->GetAzimuthAngle();
}

 Vector3 CameraManager::GetForward() const
{
	return mMainCamera.top().get()->GetForward();
}

 Vector3 CameraManager::GetForwardXZ() const
{
	return  mMainCamera.top().get()->GetForwardXZ();
}

 Vector3 CameraManager::GetRight() const
{
	return mMainCamera.top().get()->GetRight();
}

 Vector3 CameraManager::GetRightXZ() const
{
	return  mMainCamera.top().get()->GetRightXZ();
}
