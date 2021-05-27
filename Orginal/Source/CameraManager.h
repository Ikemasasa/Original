#pragma once
#include <stack>
#include <memory>

#include "lib/Vector.h"

#include "CameraBase.h"
#include "Singleton.h"

class Actor;

// singleton‚ğ’Ê‚·
class CameraManager
{
	// SceneManager‚ªstackg‚Á‚Ä‚é‚©‚çƒJƒƒ‰‚àˆê‰
	std::stack<std::shared_ptr<CameraBase>> mMainCamera;

public:
	CameraManager() = default;
	~CameraManager() = default;

	void Push(const std::shared_ptr<CameraBase>& camera) { mMainCamera.push(camera); };
	void Pop() { mMainCamera.pop(); }

	void Update(const std::shared_ptr<Actor>& target);

	DirectX::XMFLOAT4X4 GetView() const { return mMainCamera.top().get()->GetViewMatrix(); }
	DirectX::XMFLOAT4X4 GetProj() const { return mMainCamera.top().get()->GetProjectionMatrix(); }
	Vector3 GetPos() const { return mMainCamera.top().get()->GetPos(); }
	std::shared_ptr<CameraBase> GetCamera() const { return mMainCamera.top(); }

};