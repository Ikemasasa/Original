#include "CommandBehaviour.h"

#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CommandBase.h"
#include "CommandCharaSelect.h"
#include "CommandEscape.h"
#include "CommandItem.h"

CommandBehaviour::CommandBehaviour()
{
	mIcons = std::make_unique<Sprite>(L"Data/Image/Battle/command_icons.png");
	mCommandNamePlate = std::make_unique<Sprite>(L"Data/Image/Battle/command_name_plate.png");
	mCommandIndex = { 1, 1 };
}

void CommandBehaviour::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	BattleState::GetInstance().SetState(BattleState::State::COMMAND_SELECT);

	// �����ƍœK���ł���Ǝv�����ǁA�Ƃ肠�����S��
	mOldCommandIndex = mCommandIndex;

	if (Input::GetButtonTrigger(0, Input::UP))    mCommandIndex.y = Math::Max(mCommandIndex.y - 1, COMMAND_MIN_Y);
	if (Input::GetButtonTrigger(0, Input::DOWN))  mCommandIndex.y = Math::Min(mCommandIndex.y + 1, COMMAND_MAX_Y);
	if (Input::GetButtonTrigger(0, Input::RIGHT)) mCommandIndex.x = Math::Min(mCommandIndex.x + 1, COMMAND_MAX_X);
	if (Input::GetButtonTrigger(0, Input::LEFT))  mCommandIndex.x = Math::Max(mCommandIndex.x - 1, COMMAND_MIN_X);


	if (mCommandIndex.y != 1 && mOldCommandIndex.y != mCommandIndex.y)	// Y��1�ȊO�� �O���Y�ƈႤ�l�Ȃ�
	{
		mCommandIndex.x = 1;
	}
	if (mCommandIndex.x != 1 && mOldCommandIndex.x != mCommandIndex.x) // X��1�ȊO�� �O���X�ƈႤ�l�Ȃ�
	{
		mCommandIndex.y = 1;
	}


	// ����ȃ��C�A�E�g
	//    ��
	// �h �U �Z
	//    ��
	if (Input::GetButtonTrigger(0, Input::A))
	{
		if (mCommandIndex.y == 0) mNextCommand = std::make_unique<CommandItem>();
		else if (mCommandIndex.y == 2) mNextCommand = std::make_unique<CommandEscape>();
		else if (mCommandIndex.x == 0) cmdBase->SetBehaviour(CommandBase::Behaviour::GUARD);
		else if (mCommandIndex.x == 1) mNextCommand = std::make_unique<CommandCharaSelect>(Character::Type::ENEMY);
		else if (mCommandIndex.x == 2);
	}
}

void CommandBehaviour::Render()
{
	const float LeftTopX = 300.0f;
	const float LeftTopY = 350.0f;

	const Vector2 scale(ICON_SCALE, ICON_SCALE);
	const Vector2 size(ICON_SIZE, ICON_SIZE);
	const Vector2 scaleSize = Vector2(size.x * scale.x, size.y * scale.y);

	// �ォ��u����v�u�h��v�u�U���v�u�Z�v�u���v�̍��W
	Vector2 pos[] =
	{
		{ LeftTopX + scaleSize.x, LeftTopY },
		{ LeftTopX, LeftTopY + scaleSize.y },
		{ LeftTopX + scaleSize.x, LeftTopY + scaleSize.y },
		{ LeftTopX + scaleSize.x * 2, LeftTopY + scaleSize.y },
		{ LeftTopX + scaleSize.x, LeftTopY + scaleSize.y * 2 },
	};
	Vector2 selectPos = Vector2(LeftTopX + mCommandIndex.x * scaleSize.x, LeftTopY + mCommandIndex.y * scaleSize.y);

	const wchar_t* commandName[] =
	{
		L"�A�C�e��", L"�h��", L"�U��", L"�X�L��", L"������"
	};

	// �R�}���h�̖��O������v���[�g��`��
	Vector2 namePlatePos(LeftTopX, LeftTopY - mCommandNamePlate->GetSize().y * scale.y);
	mCommandNamePlate->Render(namePlatePos, scale, Vector2::ZERO, mCommandNamePlate->GetSize());

	for (int i = 0; i < COMMAND_NUM; ++i)
	{
		// �܂��x�[�X�̃A�C�R����`�悷��
		mIcons->Render(pos[i], scale, Vector2(ICON_BASE_X, 0.0f), size);

		if (pos[i] == selectPos)
		{
			// �I�𒆂̉摜��`�悷��
			mIcons->Render(selectPos, scale, Vector2(ICON_SELECT_X, 0.0f), size);
			
			float fontOffsetY = 3.0f;
			Vector2 center(mFont.GetWidth(commandName[i]) / 2.0f, 0.0f);
			mFont.RenderSet(commandName[i], namePlatePos + Vector2(mCommandNamePlate->GetSize().x / 2.0f * scale.x, fontOffsetY), center, Define::FONT_COLOR);
		}

		// ���ꂼ��̃A�C�R����`�悷��
		Vector2 texPos(i * ICON_SIZE, 0.0f);
		mIcons->Render(pos[i], scale, texPos, size);
	}

	mFont.Render();
}

void CommandBehaviour::ResetParam()
{
	// �^�[���J�n�������������ĂĂق����̂� 1
	mCommandIndex.x = 1;
	mCommandIndex.y = 1;
	mOldCommandIndex.x = 1;
	mOldCommandIndex.y = 1;
}
