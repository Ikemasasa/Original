#pragma once

struct Status;

class DamageCalculator
{
	static const int STR_DIV = 2;
	static const int VIT_DIV = 4;
	static const int WIDTH_DIV = 4;

public:
	static int CalcAttackDamage(const Status* deal, const Status* take);
	static int CalcItemDamage(int damage, const Status* take);
};