#pragma once
#include <DirectXMath.h>
#include <memory>
#include <list>

class PlayerManager;
class EnemyManager;
class Terrain;
class CameraBase;
class Shader;

class ActorManager
{
private:
	std::shared_ptr<PlayerManager> mPlayerManager;
	std::shared_ptr<EnemyManager> mEnemyManager;
	std::shared_ptr<Terrain> mTerrain;

public:
	ActorManager();
	~ActorManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	std::shared_ptr<PlayerManager> GetPlayerManager() const { return mPlayerManager; }
	std::shared_ptr<EnemyManager> GetEnemyManager() const { return mEnemyManager; }
	std::shared_ptr<Terrain> GetTerrain() const { return mTerrain; }

};