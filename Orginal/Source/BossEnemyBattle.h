#pragma once
#include "BattleCharacter.h"

class BattleCharacterManager;
class Enemy;

// ボスの戦闘用キャラクタークラス
class BossEnemyBattle : public BattleCharacter
{

public: // 関数

	// コンストラクタ
	BossEnemyBattle(const Enemy* enemy);

	// デストラクタ
	~BossEnemyBattle();

	// 初期化
	void Initialize() override;

};