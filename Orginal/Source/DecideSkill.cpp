#include "DecideSkill.h"

#include "lib/Audio.h"
#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DecideTargetChara.h"
#include "SkillData.h"

void DecideSkill::Initialize(const BattleCharacterManager* bcm)
{
	int charaID = bcm->GetMoveChara()->GetCharaID();
	mSkillSelect.Initialize(charaID, Vector2(BOARD_POS_X, BOARD_POS_Y));
}

void DecideSkill::Update(const BattleCharacterManager* bcm, CommandBase* cb)
{
	// ���ʉ���炷
	SoundPlay();

	mSkillSelect.Update();
	if (mSkillSelect.ExistSkill())
	{
		// �X�L���I��������
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			// �X�L�����擾
			const SkillData::SkillParam* param = mSkillSelect.GetSelectSkill();

			if (bcm->GetMoveChara()->GetStatus()->GetMP() >= param->useMP)
			{
				// �X�L���̃p�����[�^�ۑ�
				cb->SetSkillParam(param);

				//���̃R�}���h�����߂�
				Character::Type charaType;
				switch (param->target)
				{
				case SkillData::Target::PARTY: charaType = Character::PLAYER; break;
				case SkillData::Target::ENEMY: charaType = Character::ENEMY;  break;
				}
				mNextCommand = std::make_unique<DecideTargetChara>(charaType);
			}
			else
			{
				AUDIO.SoundStop((int)Sound::SELECT);
				AUDIO.SoundPlay((int)Sound::CANCEL);
			}

		}
	}


	// ���̉�ʂɖ߂�
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
		cb->SetSkillParam(nullptr); 
	}
}

void DecideSkill::Render()
{
	mSkillSelect.Render();
}