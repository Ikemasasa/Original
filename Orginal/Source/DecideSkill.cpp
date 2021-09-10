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
	// 効果音を鳴らす
	SoundPlay();

	mSkillSelect.Update();
	if (mSkillSelect.ExistSkill())
	{
		// スキル選択したら
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			// スキル情報取得
			const SkillData::SkillParam* param = mSkillSelect.GetSelectSkill();

			if (bcm->GetMoveChara()->GetStatus()->GetMP() >= param->useMP)
			{
				// スキルのパラメータ保存
				cb->SetSkillParam(param);

				//次のコマンドを決める
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
				Audio::SoundStop((int)Sound::SELECT);
				Audio::SoundPlay((int)Sound::CANCEL);
			}

		}
	}


	// 元の画面に戻る
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
