#pragma once
#include "ItemData.h"
#include "SkillData.h"

class BattleCharacterManager;

// �퓬���̃R�}���h�����߂���N���X
class CommandBase
{
public: // �ϐ�
	enum class Behaviour
	{
		NONE,
		USE_ITEM,
		GUARD,
		ATTACK,
		SKILL,
		ESCAPE,

		MAX
	};

protected:
	Behaviour mBehaviour = Behaviour::NONE;	// ���܂����s��
	std::vector<int> mTargetObjIDs;			// �Ώۂ�ObjID���X�g

	bool mIsUseItem = false;		  // �A�C�e�����g����
	UseItemData::Param mUseItemParam; // �A�C�e���̃p�����[�^

	bool mIsUseSkill = false;			// �X�L�����g����
	SkillData::BaseData mUseSkillParam; // �X�L���̃p�����[�^
										
public:// �֐�

	// �R���X�g���N�^
	CommandBase() = default;

	// �f�X�g���N�^
	virtual ~CommandBase() = default;

	// �X�V
	virtual void Update(const BattleCharacterManager* bcm) = 0;

	// �`��
	virtual void Render() const {};
	
	// �s���I��
	virtual void BehaviourFinished();


	// �Q�b�^�[
	bool IsUseItem() const { return mIsUseItem; }							   // �A�C�e�����g�������擾
	bool IsUseSkill() const { return mIsUseSkill; }							   // �X�L�����g�������擾
	bool IsBehaviourEnable() const { return (mBehaviour != Behaviour::NONE); } // �s�������܂��������擾

	std::vector<int> GetTargetObjIDs() const { return mTargetObjIDs; }			 // �Ώۂ�ObjID���X�g���擾
	Behaviour GetBehaviour() const { return mBehaviour; }						 // �s�����擾
	const UseItemData::Param* GetItemParam() const { return &mUseItemParam; }    // �A�C�e���̃p�����[�^�擾
	const SkillData::BaseData* GetSkillParam() const { return &mUseSkillParam; } // �X�L���̃p�����[�^�擾
	
	void SetBehaviour(const Behaviour behaviour) { mBehaviour = behaviour; }			 // �s����ݒ�
	void SetItemParam(const UseItemData::Param* param);									 // �A�C�e���̃p�����[�^��ݒ�
	void SetSkillParam(const SkillData::BaseData* param);								 // �X�L���̃p�����[�^��ݒ�
	void AddTargetObjID(const int targetObjID) { mTargetObjIDs.push_back(targetObjID); } // �Ώۂ�ObjID���X�g�ɒǉ�
};
