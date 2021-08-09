#pragma once
#include <DirectXMath.h>
#include <memory>

#include "EnemyManager.h"
#include "PlayerManager.h"

class Character;
class Player;
class Shader;

class CharacterManager
{
public:
	static const int PLAYER_MASS = 100;
	static const int ENEMY_MASS = 0;

private:
	std::unique_ptr<PlayerManager> mPlayerManager;
	std::unique_ptr<EnemyManager> mEnemyManager;

	void Collide(Character* a, Character* b);
public:
	CharacterManager();
	~CharacterManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	Player* GetMovePlayer() const { return mPlayerManager->GetMovePlayer(); }
};