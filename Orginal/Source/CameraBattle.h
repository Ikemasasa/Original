#pragma once
#include "CameraBase.h"

#include <memory>

#include "lib/Vector.h"

#include "Character.h"

/*
	SceneBattle�Ɏg���J����
	BattleState�N���X���Q�Ƃ��Ă邩�猋�\�������R�[�h�ɂȂ��Ă�
*/
class CameraBattle : public CameraBase
{
	// �萔

	// ����
	static constexpr float LERP_SPEED = 0.4f;

	// �퓬�J�n���o�֌W
public:
	static constexpr float BEGIN_START_DISTANCE = 16.0f; // �퓬�J�n���o���J�n���鎞�̃^�[�Q�b�g����̋���
private:
	static constexpr float BEGIN_END_DISTANCE = 10.0f;    // �퓬�J�n���o���I�����鎞�̃^�[�Q�b�g����̋���
	static constexpr float BEGIN_START_ZENITH = 70 * 0.01745f;   // �퓬�J�n���o���J�n����Ƃ��̓V���p(Rad)
	static constexpr float BEGIN_START_AZIMUTH = -90 * 0.01745f; // �퓬�J�n���o���J�n����Ƃ��̕��ʊp(Rad)
	static constexpr float LERP_START_FADE_ALPHA = 0.6f;   // �퓬�J�n���o���J�n����t�F�[�h��A�l
	static constexpr float BEGIN_LERP_ACCEL = 0.001f;      // �퓬�J�n���o�̐��`�⊮�̉����x

	// �R�}���h�Z���N�g�֌W
	static constexpr float COMMAND_SELECT_DIST_XZ = 7.0f;	  // �R�}���h�Z���N�g��XZ�̋���
	static constexpr float COMMAND_SELECT_HEIGHT_RATE = 2.0f; // �R�}���h�Z���N�g�̍����̊���

	// �U���֌W
	static constexpr float ATTACK_DISTANCE = 6.0f;		  // �U���̃^�[�Q�b�g����̋���
	static constexpr float ATTACK_HEIGHT_RATE = 0.5f; // �U������L�����̍����̊���


private: //�ϐ�

	// ����
	float mDistance = 0.0f;   // �^�[�Q�b�g����̋���
	float mLerpFactor = 0.0f; // ���`�⊮�̌W��
	Vector3 mCommandSelectPos = {}; // �R�}���h�I���̎��̍��W(�U����ɂ��̈ʒu�ɖ߂�)

	// �퓬�J�n���o�֌W
	float mTimer = 0.0f;		// ���o�̎��ԊǗ��̃^�C�}�[
	float mLerpVelocity = 0.0f; // ���`�⊮�̌W���̉����l
	Vector3 mBeginPos = {};		// �퓬�J�n���o�̊J�n�ʒu
	Vector3 mEndPos = {};		// �퓬�J�n���o�̏I���ʒu

	// �U���֌W
	Character::MotionType mCommandCharaMotion = Character::IDLE; // ���ݍs�����̃L�����̃��[�V����
	bool mPlAttackFinishFlag = false;							 // �U�����[�V�������I��������̃t���O
	bool mEnmAttackFinishFlag = false;							 // �U�����[�V�������I��������̃t���O

private: // �֐�
	
	// �퓬�J�n���o�̃J����
	void Begin(const BattleCharacter* commandChara);

	// �R�}���h�Z���N�g���̃J����
	void CommandSelect(const BattleCharacter* commandChara);

	// �p�[�e�B�[�L������I�ԂƂ��̃J����
	void PartySelect(const BattleCharacter* commandChara);

	// �G�L������I�ԂƂ��̃J����
	void EnemySelect(const BattleCharacter* commandChara);

	// �v���C���[���U������Ƃ��̃J����
	void PlayerAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm);

	// �G���U������Ƃ��̃J����
	void EnemyAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm);

	// ������Ƃ��̃J����
	void Escape(const BattleCharacter* commandChara);

	// �U���I���t���O�����Z�b�g
	void ResetAttackFinishFlag();

public:

	// �R���X�g���N�^
	CameraBattle();
	
	// �f�X�g���N�^
	~CameraBattle() = default;

	// �������֐�(�I�[�o�[���C�h)
	void Initialize(const Vector3& target, float distance = BEGIN_START_DISTANCE) override;

	// �X�V�֐�(�I�[�o�[���C�h)
	void Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm) override;
};