#pragma once
#include "IDecideCommand.h"

#include "Character.h"
#include "HealthGauge.h"

class BattleCharacter;
class BattleCharacterManager;
class CommandBase;
class Sprite;

/*
	�퓬���̃R�}���h�őΏۂ�I������N���X

	�N�ɉ���������Ă����̂��ŏI�I�ɂ����Ō��߂�̂ŁA
	��ԍŌ�ɒʂ�N���X
*/
class DecideTargetChara : public IDecideCommand
{
	// �萔
	static constexpr float ARROW_SCALE = 0.5f;	// ���̃X�P�[��

private: // �ϐ�
	enum Range {SINGLE, ALL};
	Range mRange = SINGLE; // �Ώۂ̐�

	std::unique_ptr<Sprite> mArrow; // ���̉摜
	Character::Type mCharaType;					 // �Ώۂ̃^�C�v
	std::vector<HealthGauge> mHealthGauges;		 // �Ώۂ̃X�e�[�^�X�Q�[�W
	std::vector<BattleCharacter*> mTargetCharas; // �Ώۂ̃L�������X�g
	std::vector<int> mEffectHandles;			 // �I���G�t�F�N�g�̃C���X�^���X�n���h��

private: // �֐�

	// 1�l�̑Ώۂ�I������
	void SelectSingleTarget(const BattleCharacterManager* bcm);

	// �S�̂̑Ώۂ�I������(����{�^����������)
	void SelectAllTarget(const BattleCharacterManager* bcm);

	// �Ώۂ̃X�e�[�^�X�Q�[�W��ݒ肷��
	void SetHealthGauge();


	// �s�����U���ɐݒ肷��
	void SetBehaviourAttack(CommandBase* cmdBase);

	// �s�����A�C�e���ɐݒ肷��
	void SetBehaviourUseItem(CommandBase* cmdBase);

	// �s�����X�L���ɐݒ肷��
	void SetBehaviourUseSkill(CommandBase* cmdBase);

public:

	// �R���X�g���N�^
	DecideTargetChara(Character::Type characterType);

	// �f�X�g���N�^
	~DecideTargetChara() = default;

	// ������
	void Initialize(const BattleCharacterManager* bcm) override;

	// �X�V
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;

	// �`��
	void Render();
};