#pragma once
#include "IDecideCommand.h"

#include "lib/Font.h"
#include "lib/TVector.h"

class Sprite;

class CommandBehaviour : public IDecideCommand
{
	static const int COMMAND_MIN_X = 0;
	static const int COMMAND_MAX_X = 2;
	static const int COMMAND_MIN_Y = 0;
	static const int COMMAND_MAX_Y = 2;
	static const int COMMAND_NUM = 5;

	static const int ICON_SIZE = 256;
	static constexpr float ICON_SCALE = 0.25f;

	static const int ICON_SELECT_X = ICON_SIZE * 5;
	static const int ICON_BASE_X = ICON_SIZE * 6;

private:
	TVector2<int> mCommandIndex;
	TVector2<int> mOldCommandIndex;
	std::unique_ptr<Sprite> mIcons;
	std::unique_ptr<Sprite> mCommandNamePlate;
	Font mFont;

public:
	CommandBehaviour();
	~CommandBehaviour() = default;

	virtual void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	virtual void Render() override;

	void ResetParam() override;
};