#include "CommandSkillSelect.h"

#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "CommandCharaSelect.h"
#include "SkillData.h"

void CommandSkillSelect::Initialize(const BattleCharacterManager* bcm)
{
	int charaID = bcm->GetMoveChara()->GetCharaID();
	mSkillSelect.Initialize(charaID, Vector2(BOARD_POS_X, BOARD_POS_Y));
}

void CommandSkillSelect::Update(const BattleCharacterManager* bcm, CommandBase* cb)
{
	if (mSkillSelect.ExistSkill())
	{
		mSkillSelect.Update();

		// スキル選択したら
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			// スキル情報取得
			const SkillData::SkillParam* param = mSkillSelect.GetSelectSkill();

			// スキルのパラメータ保存
			cb->SetSkillParam(param);

			//次のコマンドを決める
			Character::Type charaType;
			switch (param->target)
			{
			case SkillData::Target::PARTY: charaType = Character::PLAYER; break;
			case SkillData::Target::ENEMY: charaType = Character::ENEMY;  break;
			}
			mNextCommand = std::make_unique<CommandCharaSelect>(charaType);

		}
	}


	// 元の画面に戻る
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
		cb->SetSkillParam(nullptr); 
	}
}

void CommandSkillSelect::Render()
{
	mSkillSelect.Render();
}
