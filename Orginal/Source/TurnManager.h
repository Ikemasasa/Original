#pragma once
#include <memory>
#include <queue>
#include <vector>

#include "IBattleProduction.h"

class BattleCharacter;
class BattleActorManager;

// ターン進行のマネージャ
class TurnManager
{
public:
	static const int DEATH_EFFECT_SLOT = 10;
	static const int HEAL_POTION_EFFECT_SLOT = 11;
	static const int MAGIC_POTION_EFFECT_SLOT = 12;
	static const int ITEM_DAMAGE_EFFECT_SLOT = 12;

private:
	std::queue<BattleCharacter*> mOrder;
	std::unique_ptr<IBattleProduction> mProduction;
	bool mIsTurnFinished = false;
	bool mIsResult = false;

	void SortOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleActorArray);
	void BeginProduction();
	void OrganizeOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleActorArray);

public:
	void Initialize(const std::vector<std::shared_ptr<BattleCharacter>>& battleActorArray);
	void Update(const BattleActorManager* bam);
	void Render();

	void ToResult();

	// ゲッター
	BattleCharacter* GetMoveActor() const { return mOrder.front(); }
	bool IsTurnFinished() const { return mIsTurnFinished; }
	bool IsResult() const { return mIsResult; }
};