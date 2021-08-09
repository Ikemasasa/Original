#include "CommandCharaSelect.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBase.h"
#include "Item.h"
#include "Singleton.h"

CommandCharaSelect::CommandCharaSelect(Character::Type characterType)
{
	mArrow = std::make_unique<Sprite>(L"Data/Image/Battle/arrow.png");
	mCharaType = characterType;
}

void CommandCharaSelect::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// キャラタイプによってStateを変える
	if (mCharaType == Character::PLAYER)     BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);
	else if (mCharaType == Character::ENEMY) BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);

	// 生きてる同じタイプのアクターから選択する
	const std::vector<int>& ids = bcm->GetAliveCharaIDs(mCharaType);
	int max = static_cast<int>(ids.size() - 1);
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mCharaIndex = Math::Min(mCharaIndex + 1, max);
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mCharaIndex = Math::Max(mCharaIndex - 1, 0);
	mTargetChara = bcm->GetChara(ids[mCharaIndex]); // Renderでつかうから ここで代入

	// キャラ選択したら
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		cmdBase->SetTargetObjID(ids[mCharaIndex]);
		
		// アイテムが登録されているかどうかで分岐
		if (cmdBase->GetItemIndex() != -1)
		{
			// アイテム使用者のインベントリ取得
			BattleCharacter* moveChara = bcm->GetMoveChara();
			Item* moveCharaInventory = moveChara->GetInventory();
			
			// 使用アイテムが回復アイテムなら HP、MPが最大かチェック
			bool isUseable = true;
			const ItemData::ItemParam* itemParam = moveCharaInventory->GetItemParam(cmdBase->GetItemIndex());
			if (itemParam->effect == ItemData::HEAL)
			{
				//HP回復でHPがマックス, MP回復でMPがマックス なら使用不可
				if      (itemParam->hpValue > 0 && moveChara->GetStatus()->IsFullHP()) isUseable = false;
				else if (itemParam->mpValue > 0 && moveChara->GetStatus()->IsFullMP()) isUseable = false;
			}

			if (isUseable) cmdBase->SetBehaviour(CommandBase::Behaviour::USE_ITEM);
			else AUDIO.SoundPlay((int)Sound::CANCEL);// 使えないならキャンセル音
		}
		else // アイテムが登録されていない
		{
			cmdBase->SetBehaviour(CommandBase::Behaviour::ATTACK);
		}
	}

	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void CommandCharaSelect::Render()
{
	const CameraManager& camera = Singleton<CameraManager>().GetInstance();

	Vector3 targetPos = mTargetChara->GetPos();
	targetPos.y += mTargetChara->GetLocalAABB().max.y;
	Vector2 screen = targetPos.WorldToScreen(camera.GetView(), camera.GetProj());

	Vector2 scale(ARROW_SCALE, ARROW_SCALE);
	Vector2 texPos(0.0f, 0.0f);
	Vector2 size(mArrow->GetSize());
	Vector2 center(size.x / 2.0f, size.y);
	mArrow->Render(screen, scale, texPos, size, center);
}