#pragma once
#include <memory>
#include <queue>
#include <vector>

class BattleActor;
class IBattleProduction;

// ターン進行のマネージャ
class TurnManager
{
	std::queue<BattleActor*> mOrder;
	std::unique_ptr<IBattleProduction> mProduction;


public:
	void Initialize(const std::vector<BattleActor>& battleActorArray);
	void Update();
};