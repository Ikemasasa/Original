#pragma once
#include <memory>

#include "CommandBase.h"

#include "lib/Vector.h"

class Sprite;

class CommandPlayer : public CommandBase
{
	static const int ICON_SIZE_X = 128;
	static const int ICON_SIZE_Y = 128;
	static constexpr float ICON_SCALE_X = 0.5f;
	static constexpr float ICON_SCALE_Y = 0.5f;

	static const int ICON_ITEM_X    = ICON_SIZE_X * 0;
	static const int ICON_DEFENCE_X = ICON_SIZE_X * 1;
	static const int ICON_ATTACK_X  = ICON_SIZE_X * 2;
	static const int ICON_SKILL_X   = ICON_SIZE_X * 3;
	static const int ICON_ESCAPE_X  = ICON_SIZE_X * 4;
	static const int ICON_SELECT_X  = ICON_SIZE_X * 5;
	static const int ICON_CHARA_X   = ICON_SIZE_X * 6;

public:
	enum class CommandState
	{
		INIT,
		SELECT_BEHAVIOUR,
		SELECT_ITEM,
		SELECT_DEFENCE,
		SELECT_ATTACK,
		SELECT_SKILL,
		SELECT_ESCAPE,
		SELECT_ENEMY,
		SELECT_PLAYER,
	};

private:
	CommandState mCmdState = CommandState::INIT;
	int mCommandX;
	int mCommandY;
	int mSelectIndex; // アイテム、スキルの選択index
	int mTargetObjectIndex;

	std::shared_ptr<Sprite> mIcons;

public:
	CommandPlayer();
	~CommandPlayer();
	void Update(const BattleActorManager* bam) override;
};