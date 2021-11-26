#pragma once
#include "Character.h"

#include "NPCTextData.h"


/*
	�t�B�[���h��NPC�L�����N�^�[�N���X
	��b�����Ƃ��̂ǂ̃e�L�X�g���Q�Ƃ��邩�̏��������Ă���
*/
class NPC : public Character
{
	// �萔
	static const int TALK_COUNT_INIT = 1; // �b�������񐔂̏����l

private: // �ϐ�
	std::vector<NPCTextData::TextData> mTextData; // �\������e�L�X�g
	int mTalkCount; // �b����������
	bool mOldFlag;  // �O��̃t���O(������{�X�����t���O�Ƃ�������ɗv���P)

public: // �֐�

	// �R���X�g���N�^
	NPC(int charaID);
	
	// �f�X�g���N�^
	~NPC();

	// ������
	void Initialize();

	// �X�V
	void Update();

	// ���
	void Release();

	// �\������e�L�X�g���擾
	std::vector<std::wstring> GetTexts(); 
	
	// NPC�̖��O���擾
	std::wstring GetName();				  
};