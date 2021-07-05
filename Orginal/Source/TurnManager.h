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

private:
	std::queue<BattleActor*> mOrder;
	std::unique_ptr<IBattleProduction> mProduction;

	void SortOrder(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray);
	void BeginProduction();
	void OrganizeOrder(const BattleActorManager* bam);

public:
	void Initialize(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray);
	void Update(const BattleActorManager* bam);
	void Render();

	BattleActor* GetMoveActor() const { return mOrder.front(); }
	bool IsTurnFinished() const { return mProduction && mProduction->GetIsFinished(); }
};