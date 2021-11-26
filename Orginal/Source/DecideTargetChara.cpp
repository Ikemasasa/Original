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
	// 効果音を鳴らす
	SoundPlay();

	// キャラタイプによってStateを変える
	if (mCharaType == Character::PLAYER)     BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);
	else if (mCharaType == Character::ENEMY) BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);

	// ターゲットキャラ配列をクリア
	mTargetCharas.clear();

	// 範囲をチェック
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

	// ターゲット選択
	switch (mRange)
	{
	case SINGLE: SelectSingleTarget(bcm); break;
	case ALL: SelectAllTarget(bcm); break;
	}

	// ヘルスゲージ設定
	SetHealthGauge();

	// エフェクト再生
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


	// キャラ選択したら
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// ターゲットに決定する
		for (auto& target : mTargetCharas)
		{
			cmdBase->AddTargetObjID(target->GetObjID());
		}
		
		// エフェクト停止
		for (const auto& handle : mEffectHandles)
		{
			Singleton<EffectManager>().GetInstance().Stop(handle);
		}

		// スキル、アイテムが登録されているかチェック
		if		(cmdBase->IsUseItem()) SetBehaviourUseItem(cmdBase);
		else if (cmdBase->IsUseSkill())   SetBehaviourUseSkill(cmdBase);
		else							  SetBehaviourAttack(cmdBase);
	}

	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}

	// キーガイド
	KeyGuide::Instance().Add(KeyGuide::A, L"決定");
	KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
	KeyGuide::Key key[] = { KeyGuide::LEFT, KeyGuide::RIGHT };
	KeyGuide::Instance().Add(key, 2, L"カーソル移動");

}

void DecideTargetChara::Render()
{
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();

	// 矢印描画
	for (auto& target : mTargetCharas)
	{
		Vector2 screen = target->GetTopPos().WorldToScreen(view, proj);

		Vector2 scale(ARROW_SCALE, ARROW_SCALE);
		Vector2 texPos(0.0f, 0.0f);
		Vector2 size(mArrow->GetSize());
		Vector2 center(size.x / 2.0f, size.y);
		mArrow->Render(screen, scale, texPos, size, center);
	}

	// ヘルスゲージ描画
	if (mTargetCharas.size() <= 1)
	{
		for (auto& gauge : mHealthGauges)
		{
			gauge.Render();
		}
	}


	// ヘルスゲージクリア
	mHealthGauges.clear();
}

void DecideTargetChara::SelectSingleTarget(const BattleCharacterManager* bcm)
{
	// 生きてるキャラから選択する
	const std::vector<int>& ids = bcm->GetAliveObjIDs(mCharaType);
	int num = static_cast<int>(ids.size());
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mSelectIndex = (mSelectIndex + num - 1) % num;
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mSelectIndex = (mSelectIndex + 1) % num;

	// ターゲットキャラ選択
	mTargetCharas[0] = bcm->GetChara(ids[mSelectIndex]); 
}

void DecideTargetChara::SelectAllTarget(const BattleCharacterManager* bcm)
{
	// 生きてるキャラのIDリストを取得
	const std::vector<int>& ids = bcm->GetAliveObjIDs(mCharaType);

	// 生きてるキャラを全てターゲットに
	for (size_t i = 0; i < ids.size(); ++i)
	{
		mTargetCharas[i] = bcm->GetChara(ids[i]);
	}
}

void DecideTargetChara::SetHealthGauge()
{
	if (mTargetCharas.size() >= 2) return;

	// ヘルスゲージを設定する
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		// ターゲットキャラ取得
		auto& target = mTargetCharas[i];

		// キャラのトップ座標をスクリーン座標変換
		Vector2 screen = target->GetTopPos().WorldToScreen(view, proj);
		screen.y -= mArrow->GetSize().y * ARROW_SCALE;

		// セット
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
	// 使用アイテムが回復アイテムなら HP、MPが最大かチェック
	bool isUseable = true;
	const UseItemData::Param* itemParam = cmdBase->GetItemParam();
	if (itemParam->base->type == ItemData::HEAL)
	{
		for (auto& target : mTargetCharas)
		{
			//HP回復でHPがマックス, MP回復でMPがマックス なら使用不可
			if (itemParam->hpValue > 0 && target->GetStatus()->IsFullHP()) isUseable = false;
			if (itemParam->mpValue > 0 && target->GetStatus()->IsFullMP()) isUseable = false;
			if (isUseable) break; // 1人でも使えたら使える
		}
	}

	if (isUseable) cmdBase->SetBehaviour(CommandBase::Behaviour::USE_ITEM);
	else Sound::Play(Sound::CANCEL);// 使えないならキャンセル音
}

void DecideTargetChara::SetBehaviourUseSkill(CommandBase* cmdBase)
{
	cmdBase->SetBehaviour(CommandBase::Behaviour::SKILL);
}

