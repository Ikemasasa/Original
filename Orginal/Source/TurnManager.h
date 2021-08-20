#pragma once
#include <memory>
#include <queue>
#include <vector>

#include "IBattleProduction.h"

class BattleCharacter;
class BattleCharacterManager;

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
	std::vector<int> mDropItemIDs;
	bool mIsTurnFinished = false;
	bool mIsResult = false;
	bool mIsBeginnig = true;

	void SortOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray);
	void BeginProduction();
	void OrganizeOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray);

public:
	void Initialize(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray);
	void Update(const BattleCharacterManager* bcm);
	void Render();

	void ToResult();

	// ゲッター
	BattleCharacter* GetMoveChara() const { return mOrder.front(); }
	std::vector<int> GetDropItemIDs() const { return mDropItemIDs; }

	bool IsTurnFinished() const { return mIsTurnFinished; }
	bool IsResult() const { return mIsResult; }

};