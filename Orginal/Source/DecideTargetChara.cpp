#include "DecideTargetChara.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBase.h"
#include "Item.h"
#include "Singleton.h"



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
	// キャラタイプによってStateを変える
	if (mCharaType == Character::PLAYER)     BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);
	else if (mCharaType == Character::ENEMY) BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);

	// 生きてる同じタイプのアクターから選択する
	const std::vector<int>& ids = bcm->GetAliveObjIDs(mCharaType);
	int num = static_cast<int>(ids.size());
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mSelectIndex = (mSelectIndex + num - 1) % num; 
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mSelectIndex = (mSelectIndex + 1) % num;
	mTargetChara = bcm->GetChara(ids[mSelectIndex]); // Renderでつかうから ここで代入

	// キャラ選択したら
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		cmdBase->AddTargetObjID(ids[mSelectIndex]);
		
		// スキル、アイテムが登録されているかチェック
		if		(cmdBase->GetItemParam())  SetBehaviourUseItem(cmdBase);
		else if (cmdBase->GetSkillParam()) SetBehaviourUseSkill(cmdBase);
		else							   SetBehaviourAttack(cmdBase);
	}

	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void DecideTargetChara::Render()
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

void DecideTargetChara::SetBehaviourAttack(CommandBase* cmdBase)
{
	cmdBase->SetBehaviour(CommandBase::Behaviour::ATTACK);
}

void DecideTargetChara::SetBehaviourUseItem(CommandBase* cmdBase)
{
	// 使用アイテムが回復アイテムなら HP、MPが最大かチェック
	bool isUseable = true;
	const ItemData::ItemParam* itemParam = cmdBase->GetItemParam();
	if (itemParam->effect == ItemData::HEAL)
	{
		//HP回復でHPがマックス, MP回復でMPがマックス なら使用不可
		if (itemParam->hpValue > 0 && mTargetChara->GetStatus()->IsFullHP()) isUseable = false;
		else if (itemParam->mpValue > 0 && mTargetChara->GetStatus()->IsFullMP()) isUseable = false;
	}

	if (isUseable) cmdBase->SetBehaviour(CommandBase::Behaviour::USE_ITEM);
	else AUDIO.SoundPlay((int)Sound::CANCEL);// 使えないならキャンセル音
}

void DecideTargetChara::SetBehaviourUseSkill(CommandBase* cmdBase)
{
	cmdBase->SetBehaviour(CommandBase::Behaviour::SKILL);
}

