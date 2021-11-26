#pragma once
#include <memory>

#include "Character.h"
#include "Item.h"
#include "StatusData.h"

class CommandBase;
class BattleCharacterManager;

// �퓬�p�̃L�����N�^�[�N���X
class BattleCharacter : public Character
{
protected: // �ϐ�
	std::unique_ptr<CommandBase> mCommand = nullptr; // �R�}���h����N���X
	Status mStatus;									 // �X�e�[�^�X
	ItemInventory mInventory;						 // �A�C�e���̃C���x���g��

public: // �֐�

	// �R���X�g���N�^
	BattleCharacter(const Character* chara, Status status);
	
	// �f�X�g���N�^
	virtual ~BattleCharacter() = default;

	// ������(�������z)
	virtual void Initialize() = 0;

	// �X�V
	virtual void Update(const BattleCharacterManager* bcm);
	
	// �R�}���h�֌W�̉摜�Ƃ���`��
	void RenderCommand() const;

	CommandBase* GetCommand() const { return mCommand.get(); } // �R�}���h����N���X���擾
	Status* GetStatus() { return &mStatus; }				   // �X�e�[�^�X���擾
	ItemInventory* GetInventory() { return &mInventory; }	   // �A�C�e���̃C���x���g�����擾
};