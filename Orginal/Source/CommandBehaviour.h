#pragma once
#include "IDecideCommand.h"

#include "lib/TVector.h"

class Sprite;

class CommandBehaviour : public IDecideCommand
{
	static const int COMMAND_MIN_X = 0;
	static const int COMMAND_MAX_X = 2;
	static const int COMMAND_MIN_Y = 0;
	static const int COMMAND_MAX_Y = 2;

	static const int ICON_SIZE = 128;
	static constexpr float ICON_SCALE = 0.5f;

	static const int ICON_ITEM_X = ICON_SIZE * 0;
	static const int ICON_DEFENCE_X = ICON_SIZE * 1;
	static const int ICON_ATTACK_X = ICON_SIZE * 2;
	static const int ICON_SKILL_X = ICON_SIZE * 3;
	static const int ICON_ESCAPE_X = ICON_SIZE * 4;
	static const int ICON_SELECT_X = ICON_SIZE * 5;
	static const int ICON_CHARA_X = ICON_SIZE * 6;

private:
	TVector2<int> mCommandIndex;
	TVector2<int> mOldCommandIndex;
	std::unique_ptr<Sprite> mIcons;

public:
	CommandBehaviour();
	~CommandBehaviour() = default;

	virtual void Update(const BattleActorManager* bam, CommandBase* cmdBase) override;
	virtual void Render() override;
};