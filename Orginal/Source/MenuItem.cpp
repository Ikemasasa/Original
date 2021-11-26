#include "MenuItem.h"

#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Item.h"
#include "ItemData.h"
#include "KeyGuide.h"
#include "Player.h"
#include "PlayerManager.h"
#include "StatusData.h"
#include "Sound.h"

void MenuItem::Initialize(const PlayerManager* plm)
{
	// ヘルスゲージ作成
	mHealthGauge.resize(plm->GetNum());
	for (auto& gauge : mHealthGauge) gauge.Initialize();

	// キャラクター選択クラス初期化
	mCharacterSelect.Initialize(plm);

	// アイテム選択クラス初期化
	mItemSelect.Initialize();

	// 選択肢選択クラス初期化
	mSelectOptions.Initialize();

	// アイテムを未選択状態に設定
	mItemIndex = -1;
}

MenuBase::Select MenuItem::Update(PlayerManager* plm)
{
	// アイテム未選択状態なら
	if (mItemIndex == -1)
	{
		mCharacterSelect.Update(); // 誰のインベントリを参照するか決める

		mInventory = plm->GetPlayer(mCharacterSelect.GetIndex())->GetInventory(); // 参照するインベントリ保存
		mItemSelect.Update(mInventory); 

		// アイテム選択
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			int index = mItemSelect.GetIndex();
			const ItemData::BaseData* base = mInventory->GetItemParam(index);
			
			// 回復アイテムか、フィールド使用系アイテムなら使える
			if (base->type == ItemData::HEAL || base->type == ItemData::FIELD_USE)
			{
				mItemIndex = mItemSelect.GetIndex();
			}
			else
			{
				Sound::Stop(Sound::SELECT);
				Sound::Play(Sound::CANCEL);
			}
		}

		// 前の画面に戻る
		if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return Select::BACK;

		KeyGuide::Instance().Add(KeyGuide::A, L"決定");
		KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
	}
	
	// アイテム選択状態なら
	if (mItemIndex != -1)
	{
		// アイテム情報を取得
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);

		switch (baseData->type)
		{
		case ItemData::HEAL:	  UseHealItem(plm, baseData->id); break;
		case ItemData::FIELD_USE: UseFieldUseItem(baseData->id); break;
		}

		if (Input::GetButtonTrigger(0, Input::BUTTON::B) && !mIsUseFieldItem)
		{
			mItemIndex = -1; // アイテム未選択状態に戻す
		}

		// キーガイド
		KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
	}

	return MenuBase::NONE;
}

void MenuItem::Render()
{
	mCharacterSelect.Render(Vector2(BOARD_OFFSET_X + ItemSelect::ICON_OFFSET, BOARD_OFFSET_Y));
	mItemSelect.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y));

	if (mItemIndex != -1)
	{
		// アイテム情報取得
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);
		UseItemData::Param param = UseItemData::GetParam(baseData->id);

		if (baseData->type == ItemData::HEAL)
		{
			for (size_t i = 0; i < mHealthGauge.size(); ++i)
			{
				bool isSelectRender = false;

				// アイテムの範囲が全員, もしくは選択中のキャラクターのゲージなら選択画像を表示
				if (param.range == UseItemData::Range::ALL ||
					i == mCharacterSelect.GetIndex())
				{
					isSelectRender = true;
				}

				// ゲージを描画
				mHealthGauge[i].Render(isSelectRender);


			}
		}
		if (baseData->type == ItemData::FIELD_USE)
		{
			mSelectOptions.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y) + mItemSelect.GetCursorRightUpPos());
		}
	}
} 

void MenuItem::Release()
{

}

void MenuItem::UseFieldUseItem(const int itemID)
{
	// 選択肢作成
	mSelectOptions.AddTitle(L"使用しますか？");
	mSelectOptions.AddOption(L"はい");
	mSelectOptions.AddOption(L"いいえ");
	mSelectOptions.Update();

	if (mIsUseFieldItem)
	{
		// アイテム情報を取得
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);
		const FieldUseItemData::Param param = FieldUseItemData::GetParam(baseData->id);
		param.effect->Execute();

		// アイテムの処理が終了したら
		if (param.effect->IsEffectFinished())
		{
			// アイテム減らす
			mInventory->Sub(baseData->id);

			// アイテム未選択状態に戻す
			mItemIndex = -1;
		}

		return;
	}

	// 選択
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		switch (mSelectOptions.GetIndex())
		{
		case 0: mIsUseFieldItem = true; break;
		case 1: mItemIndex = -1; break;
		}
	}
}

void MenuItem::UseHealItem(const PlayerManager* plm, const int itemID)
{
	// HealthGaugeステータス設定
	for (size_t i = 0; i < plm->GetNum(); ++i)
	{
		// キャラクタIDを取得
		int charaID = plm->GetPlayer(i)->GetCharaID();

		// 座標算出
		float x = HEALTH_BOARD_X;
		float y = HEALTH_BOARD_Y + mHealthGauge[i].GetHeight() * i;
		Vector2 pos(x, y);

		// 設定
		mHealthGauge[i].Set(StatusData::GetPLStatus(charaID), pos, HealthGauge::LEFTTOP);
	}

	// 回復アイテムの情報を取得
	UseItemData::Param param = UseItemData::GetParam(itemID);

	// 対象のキャラクター選択
	if (param.range == UseItemData::SINGLE)
	{
		size_t max = plm->GetNum();
		if (Input::GetButtonTrigger(0, Input::UP))   mSelectIndex = (mSelectIndex + (max - 1)) % max;
		if (Input::GetButtonTrigger(0, Input::DOWN)) mSelectIndex = (mSelectIndex + 1) % max;
	}
	if (param.range == UseItemData::ALL)
	{
		// 全員選択なら-1
		mSelectIndex = -1;
	}


	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// ステータス取得
		std::vector<Status> targetStatus;

		// mSelectIndexが -1 なら全員選択
		if (mSelectIndex == -1)
		{
			for (size_t i = 0; i < plm->GetNum(); ++i)
			{
				Status status = StatusData::GetPLStatus(plm->GetPlayer(i)->GetCharaID());
				targetStatus.emplace_back(status);
			}
		}
		else
		{ 
			// mSelectIndexが-1じゃないなら単体
			Status status = StatusData::GetPLStatus(plm->GetPlayer(mSelectIndex)->GetCharaID());
			targetStatus.emplace_back(status);
		}

		// 回復アイテムが使えるかチェックする
		bool useable = false;
		for (const auto& status : targetStatus)
		{
			if (!status.IsFullHP() && param.hpValue > 0) useable = true; // HPがmaxじゃないかつhpv回復
			if (!status.IsFullMP() && param.mpValue > 0) useable = true; // MPがmaxじゃないかつmp回復
			if (useable) break; // 誰か1人でも使えたら使える
		}

		if (!useable) // 使えないならreturn
		{
			Audio::SoundStop((int)Sound::SELECT);
			Audio::SoundPlay((int)Sound::CANCEL);
			return;
		}

		// 回復
		for (auto& status : targetStatus)
		{
			status.AddHP(param.hpValue);
			status.AddMP(param.mpValue);

			// ステータス更新
			StatusData::SetPLStatus(status.GetID(), status);
		}
		Audio::SoundPlay((int)Sound::HEAL);

		// アイテム減らす
		mInventory->Sub(param.base->id);

		// アイテム未選択状態に戻す
		mItemIndex = -1;
	}
}
