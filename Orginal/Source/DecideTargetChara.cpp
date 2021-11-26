#include "DecideTargetChara.h"

#include <functional>

#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBase.h"
#include "EffectManager.h"
#include "Item.h"
#include "KeyGuide.h"
#include "Singleton.h"
#include "Sound.h"


DecideTargetChara::DecideTargetChara(Character::Type characterType)
{
	mCharaType = characterType;
}

void DecideTargetChara::Initialize(const BattleCharacterManager* bcm)
{
	mArrow = std::make_unique<Sprite>(L"Data/Image/Battle/arrow.png");
}

void DecideTargetChara::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// ���ʉ���炷
	SoundPlay();

	// �L�����^�C�v�ɂ����State��ς���
	if (mCharaType == Character::PLAYER)     BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);
	else if (mCharaType == Character::ENEMY) BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);

	// �^�[�Q�b�g�L�����z����N���A
	mTargetCharas.clear();

	// �͈͂��`�F�b�N
	mTargetCharas.resize(1);
	mHealthGauges.resize(1);
	mRange = SINGLE;
	if (cmdBase->IsUseItem())
	{
		const UseItemData::Param* param = cmdBase->GetItemParam();
		if (param->range == UseItemData::ALL)
		{
			size_t size = bcm->GetAliveObjIDs(mCharaType).size();
			mTargetCharas.resize(size);
			mHealthGauges.resize(size);
			mRange = ALL;
		}
	}
	else if (cmdBase->IsUseSkill())
	{
		const SkillData::BaseData* param = cmdBase->GetSkillParam();
		if (param->range == SkillData::ALL)
		{
			size_t size = bcm->GetAliveObjIDs(mCharaType).size();
			mTargetCharas.resize(size);
			mHealthGauges.resize(size);
			mRange = ALL;
		}
	}

	// �^�[�Q�b�g�I��
	switch (mRange)
	{
	case SINGLE: SelectSingleTarget(bcm); break;
	case ALL: SelectAllTarget(bcm); break;
	}

	// �w���X�Q�[�W�ݒ�
	SetHealthGauge();

	// �G�t�F�N�g�Đ�
	bool isPlay = false;
	mEffectHandles.resize(mTargetCharas.size(), -1);
	for (size_t i = 0; i < mEffectHandles.size(); ++i)
	{
		if (mEffectHandles[i] != -1)
		{
			bool isPlay = Singleton<EffectManager>().GetInstance().IsPlay(mEffectHandles[i]);
			if (!isPlay) mEffectHandles[i] = -1;
		}

		if (mEffectHandles[i] == -1)
		{
			EffectData::Param param = EffectData::GetParam(EffectData::SELECT_TARGET);
			mEffectHandles[i] = Singleton<EffectManager>().GetInstance().Play(param.slotID, mTargetCharas[i]->GetPos(), 0, param.scale, param.speed);
		}
	}


	// �L�����I��������
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// �^�[�Q�b�g�Ɍ��肷��
		for (auto& target : mTargetCharas)
		{
			cmdBase->AddTargetObjID(target->GetObjID());
		}
		
		// �G�t�F�N�g��~
		for (const auto& handle : mEffectHandles)
		{
			Singleton<EffectManager>().GetInstance().Stop(handle);
		}

		// �X�L���A�A�C�e�����o�^����Ă��邩�`�F�b�N
		if		(cmdBase->IsUseItem()) SetBehaviourUseItem(cmdBase);
		else if (cmdBase->IsUseSkill())   SetBehaviourUseSkill(cmdBase);
		else							  SetBehaviourAttack(cmdBase);
	}

	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}

	// �L�[�K�C�h
	KeyGuide::Instance().Add(KeyGuide::A, L"����");
	KeyGuide::Instance().Add(KeyGuide::B, L"�߂�");
	KeyGuide::Key key[] = { KeyGuide::LEFT, KeyGuide::RIGHT };
	KeyGuide::Instance().Add(key, 2, L"�J�[�\���ړ�");

}

void DecideTargetChara::Render()
{
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();

	// ���`��
	for (auto& target : mTargetCharas)
	{
		Vector2 screen = target->GetTopPos().WorldToScreen(view, proj);

		Vector2 scale(ARROW_SCALE, ARROW_SCALE);
		Vector2 texPos(0.0f, 0.0f);
		Vector2 size(mArrow->GetSize());
		Vector2 center(size.x / 2.0f, size.y);
		mArrow->Render(screen, scale, texPos, size, center);
	}

	// �w���X�Q�[�W�`��
	if (mTargetCharas.size() <= 1)
	{
		for (auto& gauge : mHealthGauges)
		{
			gauge.Render();
		}
	}


	// �w���X�Q�[�W�N���A
	mHealthGauges.clear();
}

void DecideTargetChara::SelectSingleTarget(const BattleCharacterManager* bcm)
{
	// �����Ă�L��������I������
	const std::vector<int>& ids = bcm->GetAliveObjIDs(mCharaType);
	int num = static_cast<int>(ids.size());
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mSelectIndex = (mSelectIndex + num - 1) % num;
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mSelectIndex = (mSelectIndex + 1) % num;

	// �^�[�Q�b�g�L�����I��
	mTargetCharas[0] = bcm->GetChara(ids[mSelectIndex]); 
}

void DecideTargetChara::SelectAllTarget(const BattleCharacterManager* bcm)
{
	// �����Ă�L������ID���X�g���擾
	const std::vector<int>& ids = bcm->GetAliveObjIDs(mCharaType);

	// �����Ă�L������S�ă^�[�Q�b�g��
	for (size_t i = 0; i < ids.size(); ++i)
	{
		mTargetCharas[i] = bcm->GetChara(ids[i]);
	}
}

void DecideTargetChara::SetHealthGauge()
{
	if (mTargetCharas.size() >= 2) return;

	// �w���X�Q�[�W��ݒ肷��
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		// �^�[�Q�b�g�L�����擾
		auto& target = mTargetCharas[i];

		// �L�����̃g�b�v���W���X�N���[�����W�ϊ�
		Vector2 screen = target->GetTopPos().WorldToScreen(view, proj);
		screen.y -= mArrow->GetSize().y * ARROW_SCALE;

		// �Z�b�g
		mHealthGauges[i].Set(*target->GetStatus(), screen, HealthGauge::MIDBOTTOM);
	}
}

void DecideTargetChara::SetBehaviourAttack(CommandBase* cmdBase)
{
	cmdBase->SetBehaviour(CommandBase::Behaviour::ATTACK);

	if (mCharaType == Character::ENEMY) BattleState::GetInstance().SetState(BattleState::State::PLAYER_ATTACK);
}

void DecideTargetChara::SetBehaviourUseItem(CommandBase* cmdBase)
{
	// �g�p�A�C�e�����񕜃A�C�e���Ȃ� HP�AMP���ő傩�`�F�b�N
	bool isUseable = true;
	const UseItemData::Param* itemParam = cmdBase->GetItemParam();
	if (itemParam->base->type == ItemData::HEAL)
	{
		for (auto& target : mTargetCharas)
		{
			//HP�񕜂�HP���}�b�N�X, MP�񕜂�MP���}�b�N�X �Ȃ�g�p�s��
			if (itemParam->hpValue > 0 && target->GetStatus()->IsFullHP()) isUseable = false;
			if (itemParam->mpValue > 0 && target->GetStatus()->IsFullMP()) isUseable = false;
			if (isUseable) break; // 1�l�ł��g������g����
		}
	}

	if (isUseable) cmdBase->SetBehaviour(CommandBase::Behaviour::USE_ITEM);
	else Sound::Play(Sound::CANCEL);// �g���Ȃ��Ȃ�L�����Z����
}

void DecideTargetChara::SetBehaviourUseSkill(CommandBase* cmdBase)
{
	cmdBase->SetBehaviour(CommandBase::Behaviour::SKILL);
}

