#pragma once
#include <DirectXMath.h>
#include <memory>

#include "EnemyManager.h"
#include "NPCManager.h"
#include "NPCTalk.h"
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
	std::unique_ptr<NPCManager> mNPCManager;
	std::unique_ptr<NPCTalk> mNPCTalk;

	void Collide(Character* a, Character* b);
	void CollideObject(Character* move, Character* object); 
public:
	CharacterManager();
	~CharacterManager() = default;

	void Initialize();
	void Update();
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);
	void RenderUI();

	void CollideNPC();
	void CollideEnemy();
	void TalkCheck();

	Player* GetMovePlayer() const { return mPlayerManager->GetMovePlayer(); }
	EnemyManager* GetEnemyManager() const { return mEnemyManager.get(); }
};