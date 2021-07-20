#pragma once
#include <DirectXMath.h>
#include <memory>
#include <list>

#include "EnemyManager.h"
#include "PlayerManager.h"

class CameraBase;
class Shader;
#include "Terrain.h"
class Terrain;

class ActorManager
{
private:
	std::unique_ptr<PlayerManager> mPlayerManager;
	std::unique_ptr<EnemyManager> mEnemyManager;
	std::unique_ptr<Terrain> mTerrain;

public:
	ActorManager();
	~ActorManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	Player* GetMovePlayer() { return mPlayerManager->GetMovePlayer(); }
};