#include "DamageCalculator.h"

#include "lib/Random.h"

#include "StatusData.h"

int DamageCalculator::CalcAttackDamage(const Status* deal, const Status* take)
{
	int atk = deal->GetAtk();
	int def = take->GetDef();

	int damage = atk / STR_DIV - def / VIT_DIV; // ��b�_���[�W
	if (damage < 0) damage = 0;
	int sign = (Random::Rand() % 2 == 0) ? -1 : 1; // �U�ꕝ�̕���
	int width = damage / WIDTH_DIV + 1; // �_���[�W�̐U�ꕝ�̍ő�l
	damage = damage + (rand() % width * sign);

	if (damage < 0) damage = 0;

	return damage;
}

int DamageCalculator::CalcItemDamage(int damage, const Status* take)
{
	int def = take->GetDef();

	damage -= def / VIT_DIV; // ����͓K�p
	int sign = (Random::Rand() % 2 == 0) ? -1 : 1; // �U�ꕝ�̕���
	int width = damage / WIDTH_DIV + 1; // �_���[�W�̐U�ꕝ�̍ő�l
	damage = damage + (Random::Rand() % width * sign); // �U�ꕝ�K�p

	return damage;
}

std::vector<int> DamageCalculator::CalcSkillDamage(const Status* deal, const Status* take, float rate, int num)
{
	int atk = deal->GetAtk() * rate;
	int def = take->GetDef();
	std::vector<int> ret;
	for (int i = 0; i < num; ++i)
	{
		int damage = atk / STR_DIV - def / VIT_DIV; // ��b�_���[�W
		if (damage < 0) damage = 0;
		int sign = (Random::Rand() % 2 == 0) ? -1 : 1; // �U�ꕝ�̕���
		int width = damage / WIDTH_DIV + 1; // �_���[�W�̐U�ꕝ�̍ő�l
		damage = damage + (rand() % width * sign);
		if (damage < 0) damage = 0;
		ret.push_back(damage);
	}

	return ret;
}
