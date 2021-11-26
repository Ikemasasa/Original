#pragma once
#include <memory>
#include <queue>
#include <vector>

#include "HealthGauge.h"
#include "IBattleProduction.h"

class BattleCharacter;
class BattleCharacterManager;

// ターン進行のマネージャクラス
class TurnManager
{
public: // 定数
	static constexpr float HEALTH_X = Define::SCREEN_WIDTH - 5.0f;
	static constexpr float HEALTH_Y = 500.0f;

private:
	std::queue<BattleCharacter*> mOrder;
	std::unique_ptr<IBattleProduction> mProduction;
	std::vector<int> mDropItemIDs;

	std::vector<HealthGauge> mHealthGauges;
	bool mIsTurnFinished = false;

	void SortOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray);
	void AdvanceOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray);
	void BeginProduction(const BattleCharacterManager* bcm);

public:
	void Initialize(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray);
	void Update(const BattleCharacterManager* bcm);
	void Render();

	// ゲッター
	BattleCharacter* GetMoveChara() const { return mOrder.front(); }
	std::vector<int> GetDropItemIDs() const { return mDropItemIDs; }

	bool IsTurnFinished() const { return mIsTurnFinished; }
};