#include "MenuItem.h"

#include <map>

#include "lib/Audio.h"
#include "lib/ConvertString.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Texture.h"

#include "Item.h"
#include "ItemData.h"
#include "MenuCharacterHealth.h"
#include "PlayerManager.h"
#include "Singleton.h"

void MenuItem::Initialize(const PlayerManager* plm)
{
	mCharacterHealth.Initialize(plm, Vector2(HEALTH_PLATE_X, HEALTH_PLATE_Y));
	mCharacterSelect.Initialize(plm);
	mItemSelect.Initialize();
	mItemIndex = -1;
}

MenuBase::Select MenuItem::Update(const PlayerManager* plm)
{
	if (mItemIndex == -1)
	{
		mCharacterSelect.Update(); // 誰のインベントリを参照するか決める

		mInventory = plm->GetPlayers()[mCharacterSelect.GetIndex()]->GetInventory(); // 参照するインベントリ保存
		mItemIndex = mItemSelect.Update(mInventory); // アイテム選択

		// 前の画面に戻る
		if (Input::GetButtonTrigger(0, Input::BUTTON::B))	
			return Select::BACK;
	}
	else 
	{
		mCharacterHealth.Update();

		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			int charaindex = mCharacterHealth.GetSelectIndex();
			int id = plm->GetPlayers()[charaindex]->GetCharaID();

			// ステータス更新
			ItemData::ItemParam param = mInventory->GetAll()[mItemIndex];
			Status plStatus = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(id);

			bool isHeal = false;
			if (!plStatus.IsFullHP() && param.hpValue > 0) isHeal = true; // HPがmaxじゃないかつhp回復
			if (!plStatus.IsFullMP() && param.mpValue > 0) isHeal = true; // MPがmaxじゃないかつmp回復
			if (isHeal) // 回復
			{
				plStatus.SetHP(plStatus.hp + param.hpValue);
				plStatus.SetMP(plStatus.mp + param.mpValue);
				Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(id, plStatus); //ステータス更新
				mInventory->Sub(mItemIndex); // アイテム減らす

				AUDIO.SoundPlay((int)Sound::HEAL);
			}
			else // 回復しない
			{
				AUDIO.SoundPlay((int)Sound::CANCEL);
			}

		}
		else if(Input::GetButtonTrigger(0, Input::BUTTON::B))
		{
			mItemIndex = -1;
		}
	}

	return MenuBase::NONE;
}

void MenuItem::Render()
{
	mCharacterSelect.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y));
	mItemSelect.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y));

	if (mItemIndex != -1)
	{
		mCharacterHealth.Render(true);
	}
} 

void MenuItem::Release()
{

}