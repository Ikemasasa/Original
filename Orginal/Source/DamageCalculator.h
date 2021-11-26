#pragma once
#include <vector>

struct Status;

// �_���[�W�v�Z�N���X
class DamageCalculator
{
	// �萔
	static const int STR_DIV = 2;	// �U���͂����鐔
	static const int VIT_DIV = 4;	// �h��͂����鐔
	static const int WIDTH_DIV = 7; // �_���[�W�̐U�ꕝ�����鐔

public:
	static constexpr float GUARD_BUFF_RATE = 1.5f; // �K�[�h�̖h��͏㏸����

public: // �֐�

	// �ʏ�U���ɂ��_���[�W�v�Z
	static int CalcAttackDamage(const Status* deal, const Status* take);
	
	// �A�C�e���_���[�W�ɂ��_���[�W�v�Z
	static int CalcItemDamage(int damage, const Status* take);

	// �X�L���_���[�W�ɂ��_���[�W�v�Z
	static std::vector<int> CalcSkillDamage(const Status* deal, const Status* take, float rate, int num);
};