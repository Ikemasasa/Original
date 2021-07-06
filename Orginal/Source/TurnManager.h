#pragma once
#include <memory>
#include <queue>
#include <vector>

#include "IBattleProduction.h"

class BattleActor;
class BattleActorManager;

// �^�[���i�s�̃}�l�[�W��
class TurnManager
{
public:
	static const int DEATH_EFFECT_SLOT = 10;

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

	// �Q�b�^�[
	BattleActor* GetMoveActor() const { return mOrder.front(); }
	bool IsTurnFinished() const { return mIsTurnFinished; }
	bool IsResult() const { return mIsResult; }
};