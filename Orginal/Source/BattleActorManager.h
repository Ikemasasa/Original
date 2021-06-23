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

class BattleActorManager
{
public:
	static const int BATTLEACTOR_MAX = 12;
	static const int BATTLEACTOR_KIND = 2; // Player, EnemyÇÃ2Ç¬ 

	static constexpr float ENEMY_POS_Z = 5.0f;
	static constexpr float PLAYER_POS_Z = -5.0f;

private:
	std::vector<std::unique_ptr<BattleActor>> mBActors;
	std::vector<int> mObjectIDs[BATTLEACTOR_KIND];

	BattleActor* mMoveActor = nullptr;
	std::queue<BattleActor*> mOrder;

	ProductionBattle mProduction;

	void SortOrder();
	void DecideMoveActor();
	bool CheckBattleFinish();
	int CalcDamage(const Status* deal, Status* take);

	void PlayerCreateAndRegister(Player* pl);
	void EnemyCreateAndRegister(Enemy* enm);
public:	
	BattleActorManager(PlayerManager* player, Enemy* enemy);
	~BattleActorManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);


	// ÉQÉbÉ^Å[
	BattleActor* GetMoveActor() const { return mMoveActor; }
	const std::vector<int>& GetObjectIDs(Actor::Type kind) const { return mObjectIDs[kind]; }
	BattleActor* GetActor(int objectID) const { return mBActors[objectID].get(); }
};