#pragma once
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>

#include "Actor.h"
#include "BattleActor.h"
#include "ManagerBase.h"
#include "ProductionBattle.h"

class Enemy;
class Player;

class BattleActorManager
{
public:
	static const int BATTLEACTOR_MAX = 12;
	static const int BATTLEACTOR_KIND = 2; // Player, EnemyÇÃ2Ç¬ 

	static constexpr float ENEMY_POS_Z = 5.0f;
	static constexpr float PLAYER_POS_Z = -5.0f;

private:
	std::vector<std::shared_ptr<BattleActor>> mBActors;
	std::vector<int> mObjectIDs[BATTLEACTOR_KIND];

	std::shared_ptr<BattleActor> mMoveActor = nullptr;
	std::queue<std::shared_ptr<BattleActor>> mOrder;

	ProductionBattle mProduction;

	void SortOrder();
	void DecideMoveActor();
	bool CheckBattleFinish();
	int CalcDamage(const Status* deal, Status* take);
	void CreateAndRegister(const std::shared_ptr<Actor>& actor);
public:	
	BattleActorManager(const std::shared_ptr<Player>& player, const std::shared_ptr<Enemy>& enemy);
	~BattleActorManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);



	// ÉQÉbÉ^Å[
	std::shared_ptr<BattleActor>& GetMoveActor() { return mMoveActor; }
	const std::vector<int>& GetObjectIDs(Actor::Type kind) const { return mObjectIDs[kind]; }
	std::shared_ptr<BattleActor> GetActor(int objectID) const { return mBActors[objectID]; }
};