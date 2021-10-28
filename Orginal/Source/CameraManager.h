#pragma once
#include <stack>
#include <memory>

#include "lib/Vector.h"

#include "CameraBase.h"
#include "Singleton.h"

class Character;
class BattleCharacter;
class BattleCharacterManager;

// singletonを通す
class CameraManager
{
	// SceneManagerがstack使ってるからカメラも一応
	std::stack<std::shared_ptr<CameraBase>> mMainCamera;

public:
	CameraManager() = default;
	~CameraManager() = default;

	void Push(const std::shared_ptr<CameraBase>& camera) { mMainCamera.push(camera); };
	void Pop() { if(!mMainCamera.empty()) mMainCamera.pop(); }

	void Initialize(const Character* target);
	void Update(const Character* target);
	void Update(const BattleCharacter* target, const BattleCharacterManager* bcm);
	void Shake(float duration, float strength);


	// ゲッター
	Matrix GetView() const { return mMainCamera.top().get()->GetViewMatrix(); }
	Matrix GetProj() const { return mMainCamera.top().get()->GetProjectionMatrix(); }
	Vector3 GetPos() const { return mMainCamera.top().get()->GetPos(); }
	Vector3 GetAngle() const { return mMainCamera.top().get()->GetAngle(); }
	Vector3 GetFrontVector() const { return mMainCamera.top().get()->GetForward(); }
	Vector3 GetRightVector() const { return mMainCamera.top().get()->GetRight(); }
};