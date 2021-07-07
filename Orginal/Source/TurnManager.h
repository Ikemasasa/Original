#pragma once
#include <memory>
#include <queue>
#include <vector>

#include "IBattleProduction.h"

class BattleActor;
class BattleActorManager;

// ターン進行のマネージャ
class TurnManager
{
public:
	static const int DEATH_EFFECT_SLOT = 10;
	static const int HEAL_POTION_EFFECT_SLOT = 11;
	static const int MAGIC_POTION_EFFECT_SLOT = 12;

private:
	std::queue<BattleActor*> mOrder;
	std::unique_ptr<IBattleProduction> mProduction;
	bool mIsTurnFinished = false;
	bool mIsResult = false;

	void SortOrder(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray);
	void BeginProduction();
	void OrganizeOrder(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray);

public:
	void Initialize(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray);
	void Update(const BattleActorManager* bam);
	void Render();

	void ToResult();

	// ゲッター
	BattleActor* GetMoveActor() const { return mOrder.front(); }
	bool IsTurnFinished() const { return mIsTurnFinished; }
	bool IsResult() const { return mIsResult; }
};