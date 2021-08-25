#pragma once
#include <memory>

#include "lib/Font.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "IDecideCommand.h"

class DecideEscape : public IDecideCommand
{
	static constexpr float BOARD_POS_X = Define::SCREEN_WIDTH / 2.0f;
	static constexpr float BOARD_POS_Y = Define::SCREEN_HEIGHT / 2.0f;

	static constexpr float FIRST_OFFSET_Y = 12.0f;
	static constexpr float ADD_OFFSET_Y = 32.0f;

	std::unique_ptr<Sprite> mBoard;
	Font mFont;

public:
	DecideEscape() = default;
	~DecideEscape() = default;

	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render();
};