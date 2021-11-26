#pragma once
#include "BattleCharacter.h"

class Enemy;

// 敵の戦闘用キャラクタークラス
class EnemyBattle : public BattleCharacter
{

public: // 関数

	// コンストラクタ
	EnemyBattle(const Enemy* enemy);

	// デストラクタ
	~EnemyBattle() = default;

	// 初期化
	void Initialize() override;
};