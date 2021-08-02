#pragma once
#include <DirectXMath.h>
#include <memory>
#include <list>

#include "EnemyManager.h"
#include "PlayerManager.h"
#include "Terrain.h"

class CameraBase;
class Shader;

class CharacterManager
{
private:
	std::unique_ptr<PlayerManager> mPlayerManager;
	std::unique_ptr<EnemyManager> mEnemyManager;

public:
	CharacterManager();
	~CharacterManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	Player* GetMovePlayer() { return mPlayerManager->GetMovePlayer(); }
};