#pragma once
#include <memory>

// �O���錾
class BattleCharacter;
class BattleCharacterManager;
class CommandBase;

/*
	�퓬���̃R�}���h�I���̊��N���X
*/
class IDecideCommand
{
public:
	enum class CmdState
	{
		ITEM,
		SELECT_PARTY,
		SELECT_ENEMY,

		MAX,
		BACK,
		NONE,
	};

protected: // �ϐ�
	std::unique_ptr<IDecideCommand> mNextCommand; // ���̃R�}���h
	int mSelectIndex = 0;						  // �I�𒆂̏ꏊ
	bool mIsBackState = false;					  // �R�}���h��1�߂���

public: // �֐�

	// �R���X�g���N�^
	IDecideCommand() = default;

	// �f�X�g���N�^
	virtual ~IDecideCommand() = default;

	// ������(�������z)
	virtual void Initialize(const BattleCharacterManager* bcm) = 0;

	// �X�V(�������z)
	virtual void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) = 0;
	
	// �`��
	virtual void Render() {}

	// ����ƃL�����Z���̌��ʉ��Đ�(�قƂ�ǂ̏ꏊ�ŏ�������֐���)
	void SoundPlay(); 

	// �p�����[�^���Z�b�g
	virtual void ResetParam() {};

	// �R�}���h��1�߂����擾
	bool IsBackState() const { return mIsBackState; }

	// ���ݑI�𒆂̏ꏊ���擾
	int GetSelectIndex() const { return mSelectIndex; }

	// ���̃R�}���h�̃|�C���^���擾
	IDecideCommand* GetNextCommandState() { return mNextCommand.release(); } 
};