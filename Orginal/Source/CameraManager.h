#pragma once
#include <stack>
#include <memory>

#include "lib/Vector.h"

#include "CameraBase.h"
#include "Singleton.h"

class Character;

// singletonを通す
class CameraManager
{
	// SceneManagerがstack使ってるからカメラも一応
	std::stack<std::shared_ptr<CameraBase>> mMainCamera;

public:
	CameraManager() = default;
	~CameraManager() = default;

	void Push(const std::shared_ptr<CameraBase>& camera) { mMainCamera.push(camera); };
	void Pop() { mMainCamera.pop(); }

	void Update(const Character* target);


	// ゲッター
	DirectX::XMFLOAT4X4 GetView() const { return mMainCamera.top().get()->GetViewMatrix(); }
	DirectX::XMFLOAT4X4 GetProj() const { return mMainCamera.top().get()->GetProjectionMatrix(); }
	Vector3 GetPos() const { return mMainCamera.top().get()->GetPos(); }
	Vector3 GetAngle() const { return mMainCamera.top().get()->GetAngle(); }
	Vector3 GetFrontVector() const { return mMainCamera.top().get()->GetFrontVector(); }
	Vector3 GetRightVector() const { return mMainCamera.top().get()->GetRightVector(); }
};