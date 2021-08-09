#pragma once
#include <memory>

#include "lib/Font.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "IDecideCommand.h"

class CommandEscape : public IDecideCommand
{
	static constexpr float PLATE_POS_X = Define::SCREEN_WIDTH / 2.0f;
	static constexpr float PLATE_POS_Y = Define::SCREEN_HEIGHT / 2.0f;

	static constexpr float FIRST_OFFSET_Y = 12.0f;
	static constexpr float ADD_OFFSET_Y = 32.0f;

	std::unique_ptr<Sprite> mBoard;
	Font mFont;

public:
	CommandEscape();
	~CommandEscape();

	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render();
};