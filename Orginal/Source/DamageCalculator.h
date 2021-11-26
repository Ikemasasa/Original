#pragma once
#include <vector>

struct Status;

// ダメージ計算クラス
class DamageCalculator
{
	// 定数
	static const int STR_DIV = 2;	// 攻撃力を割る数
	static const int VIT_DIV = 4;	// 防御力を割る数
	static const int WIDTH_DIV = 7; // ダメージの振れ幅を割る数

public:
	static constexpr float GUARD_BUFF_RATE = 1.5f; // ガードの防御力上昇割合

public: // 関数

	// 通常攻撃によるダメージ計算
	static int CalcAttackDamage(const Status* deal, const Status* take);
	
	// アイテムダメージによるダメージ計算
	static int CalcItemDamage(int damage, const Status* take);

	// スキルダメージによるダメージ計算
	static std::vector<int> CalcSkillDamage(const Status* deal, const Status* take, float rate, int num);
};