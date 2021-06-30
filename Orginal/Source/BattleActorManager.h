#pragma once
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>

#include "Actor.h"
#include "BattleActor.h"
#include "ProductionBattle.h"

class Enemy;
class Player;
class PlayerManager;

// ターン進行の部分を後々別にしたい

class BattleActorManager
{
public:
	static const int BATTLEACTOR_MAX = 12;
	static const int BATTLEACTOR_KIND = 2; // Player, Enemyの2つ 

	static constexpr float ENEMY_POS_Z = 5.0f;
	static constexpr float PLAYER_POS_Z = -5.0f;

private:
	std::vector<std::unique_ptr<BattleActor>> mBActors;
	std::vector<int> mAliveActorIDs[BATTLEACTOR_KIND];

	BattleActor* mMoveActor = nullptr;
	std::queue<BattleActor*> mOrder;

	ProductionBattle mProduction;

	void SortOrder();
	void DecideMoveActor();
	bool CheckBattleFinish();
	void OrganizeActor();
	int CalcDamage(const Status* deal, Status* take);

	void PlayerCreateAndRegister(Player* pl);
	void EnemyCreateAndRegister(Enemy* enm);
public:	
	BattleActorManager(PlayerManager* player, Enemy* enemy);
	~BattleActorManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);


	// ゲッター
	BattleActor* GetMoveActor() const { return mMoveActor; }
	const std::vector<int>& GetAliveActorIDs(Actor::Type kind) const { return mAliveActorIDs[kind]; }
	BattleActor* GetActor(int objectID) const { return mBActors[objectID].get(); }
};