#pragma once
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>

#include "BattleCharacter.h"
#include "DropItemShower.h"
#include "SceneBattle.h"

// �O���錾
class Enemy;
class Player;
class PlayerManager;
class TurnManager;
class Matrix;

// �퓬�p�̃L�����N�^�[�}�l�[�W���N���X
class BattleCharacterManager
{
	// �萔
	static const int BATTLECHARA_MAX = 12; // �퓬�L�����̍ő吔(���ɈӖ��͂Ȃ�)
	static const int BATTLECHARA_KIND = 2; // �퓬�L�����̎��(�v���C���A�G)

public:
	static constexpr float PLAYER_POS_Z = -5.0f; // �v���C���[��Z���W
	static constexpr float ENEMY_POS_Z = 5.0f;   // �G��Z���W

	static constexpr float POS_MIN_X = -5.0f; // X���W�̍Œ�l
	static constexpr float POS_MAX_X =  5.0f; // X���W�̍ō��l


private: // �ϐ�
	std::vector<std::shared_ptr<BattleCharacter>> mBCharacters; // �퓬�p�L�������X�g
	std::vector<int> mAliveObjIDs[BATTLECHARA_KIND];			// �������Ă���L������ObjID���X�g
	std::vector<int> mDropItemIDs;								// �h���b�v�A�C�e����ID���X�g
	BattleCharacter* mMoveChara = nullptr;						// ���݃R�}���h�I�𒆂̃L�����N�^�[
	int mPlayerNum;												// �v���C���[�̐�

	DropItemShower mDropItemShower;								// �h���b�v�A�C�e���\���N���X

private: // �֐�

	// �v���C���[�����X�g�ɒǉ�
	void PlayerCreateAndRegister(Player* pl);

	// �G�����X�g�ɒǉ�
	void EnemyCreateAndRegister(Enemy* enm);
public:	

	// �R���X�g���N�^
	BattleCharacterManager(PlayerManager* pm, Enemy* enemy);

	// �f�X�g���N�^
	~BattleCharacterManager() = default;

	// ������
	void Initialize();

	// �X�V
	void Update(const TurnManager* turnManager);

	// �`��
	void Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir);

	// UI�`��
	void RenderUI();


	// �L�����N�^�[�𐮗��A�̗͂�0�ɂȂ����L������mAliveObjIDs�������
	void OrganizeCharacter();

	// mAliveObjIDs����A�o�g���I���̔��������
	SceneBattle::Result CheckBattleFinish(); 

	// �Q�b�^�[
	// ���݃R�}���h�I�𒆂̃L�������擾
	BattleCharacter* GetMoveChara() const { return mMoveChara; }

	// �����Ă�L�����N�^�[���X�g���擾
	const std::vector<int>& GetAliveObjIDs(Character::Type kind) const { return mAliveObjIDs[kind]; }

	// ������ID�̃L�����N�^�\��
	BattleCharacter* GetChara(int objectID) const { return mBCharacters[objectID].get(); }

	// �L�����N�^�\���X�g���擾
	const std::vector<std::shared_ptr<BattleCharacter>>& GetBCharacters() const { return mBCharacters; }

	// �h���b�v�A�C�e����ID���X�g���擾
	const std::vector<int>& GetDropItemIDs() const { return mDropItemIDs; }
};