#pragma once
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>

#include "Actor.h"
#include "BattleActor.h"
#include "CharacterHealth.h"
#include "ProductionBattle.h"
#include "TurnManager.h"

class Enemy;
class Player;
class PlayerManager;

// ターン進行の部分を後々別にしたい

class BattleActorManager
{
	enum Result
	{
		NONE,
		PLAYER_WIN,
		PLAYER_LOSE,
	};

public:
	static constexpr float HEALTH_PLATE_X = 900.0f;
	static constexpr float HEALTH_PLATE_Y = 0.0f;

	static const int BATTLEACTOR_MAX = 12;
	static const int BATTLEACTOR_KIND = 2; // Player, Enemyの2つ 

	static constexpr float ENEMY_POS_Z = 5.0f;
	static constexpr float PLAYER_POS_Z = -5.0f;

private:
	std::vector<std::shared_ptr<BattleActor>> mBActors;
	std::vector<int> mAliveActorIDs[BATTLEACTOR_KIND];
	int mPlayerNum;

	CharacterHealth mCharacterHealth;
	TurnManager mTurnManager;

	bool mIsResult;
	Enemy* mHitEnemy = nullptr; // fieldで当たった敵

	Result CheckBattleFinish();
	void OrganizeActor();

	void PlayerCreateAndRegister(Player* pl);
	void EnemyCreateAndRegister(Enemy* enm);
public:	
	BattleActorManager(PlayerManager* player, Enemy* enemy);
	~BattleActorManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	// ゲッター
	BattleActor* GetMoveActor() const { return mTurnManager.GetMoveActor(); }
	const std::vector<int>& GetAliveActorIDs(Actor::Type kind) const { return mAliveActorIDs[kind]; }
	BattleActor* GetActor(int objectID) const { return mBActors[objectID].get(); }
	const std::vector<std::shared_ptr<BattleActor>>& GetBActors() const { return mBActors; }
};