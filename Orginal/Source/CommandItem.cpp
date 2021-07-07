#include "CommandItem.h"

#include "lib/Input.h"

#include "BattleActorManager.h"
#include "CommandBase.h"
#include "CommandCharaSelect.h"
#include "Define.h"
#include "Item.h"


CommandItem::CommandItem()
{
	mItemSelect.Initialize();
}

CommandItem::~CommandItem()
{
}

void CommandItem::Update(const BattleActorManager* bam, CommandBase* cmdBase)
{
	const BattleActor* moveActor = bam->GetMoveActor();

	int itemIndex = mItemSelect.Update(moveActor->GetInventory());
	const int noSelect = -1;
	if (noSelect != itemIndex)
	{
		// �A�C�e���I��������
		const ItemData::ItemParam* param = moveActor->GetInventory()->GetItemParam(itemIndex);
		cmdBase->SetItemParam(param);

		// �A�C�e���̑Ώۂɂ���Ăǂ����̂�����I�Ԃ����߂�
		Actor::Type charaType;
		if (param->target == ItemData::Target::PARTY) charaType = Actor::PLAYER;
		else if (param->target == ItemData::Target::ENEMY) charaType = Actor::ENEMY;
		
		// ���̃R�}���h
		mNextCommand = std::make_unique<CommandCharaSelect>(charaType);
	}

	// CommandBehaviour�ɖ߂�
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void CommandItem::Render() const
{
	const Vector2 boardOffset(Define::SCREEN_WIDTH / 10.0f, Define::SCREEN_HEIGHT / 5.0f);
	mItemSelect.Render(boardOffset);
}
