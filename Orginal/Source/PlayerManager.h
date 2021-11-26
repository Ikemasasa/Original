#pragma once
#include <vector>

#include "EquipmentInventory.h"
#include "Item.h"

class CameraBase;
class Matrix;
class Player;
class Shader;
class Vector4;

/*
	�v���C���[�}�l�[�W��
	�v���C���[�̐����A�X�V�A�`������Ă���
*/
class PlayerManager
{
	// �萔
	static const int INVINCIBLE_SECOND = 2; // ���G����

public:
	enum PlList
	{
		ERIKA = DataBase::PL_ID_START,
		PALADIN,

		PL_ID_MAX
	};
	static const int ALL_PLAYER_NUM = PL_ID_MAX - DataBase::PL_ID_START;

private: // �ϐ�
	std::vector<std::unique_ptr<Player>> mPlayers; // �v���C���[�̃��X�g
	Player* mMovePlayer = nullptr;			       // �t�B�[���h�œ����v���C���[
	bool mIsInvincible = false;					   // ���G���Ԃ��ǂ���
	float mInvincibleTimer = 0;					   // ���G���Ԃ̃^�C�}�[

	EquipmentInventory mEquipmentInventory;		   // �����i�̃C���x���g��

public: // �֐�

	// �R���X�g���N�^
	PlayerManager();

	//�f�X�g���N�^
	~PlayerManager();

	// ����
	void Create(int charaID);

	// ������
	void Initialize();

	// �X�V
	void Update(const bool isTalking);

	// �`��
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// �Q�b�^�[
	Player* GetMovePlayer() const { return mMovePlayer; }
	Player* GetPlayer(const size_t index) const { return mPlayers[index].get(); }
	size_t GetNum() const { return mPlayers.size(); }
	bool IsInvincible() const { return mIsInvincible; }
	EquipmentInventory* GetEquipmentInventory() { return &mEquipmentInventory; }

	// �Z�b�^�[
	void EnableInvincible() { mIsInvincible = true; }

};