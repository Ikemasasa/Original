#pragma once
#include "IDecideCommand.h"

#include "lib/Font.h"
#include "lib/TVector.h"

class Sprite;

class DecideBehaviour : public IDecideCommand
{
	static const int COMMAND_MIN_X = 0;
	static const int COMMAND_MAX_X = 2;
	static const int COMMAND_MIN_Y = 0;
	static const int COMMAND_MAX_Y = 2;
	static const int COMMAND_NUM = 5;
	static const int COMMAND_VERTICAL = 3;

	static constexpr float ICON_SIZE = 256.0f;
	static constexpr float ICON_SCALE = 0.25f;
	static constexpr float ICON_SELECT_X = ICON_SIZE * 5;
	static constexpr float ICON_BASE_X   = ICON_SIZE * 6;

private:
	TVector2<int> mCommandIndex;
	std::unique_ptr<Sprite> mIcons;
	std::unique_ptr<Sprite> mCommandNameBoard;
	Font mFont;

public:
	DecideBehaviour();
	~DecideBehaviour() = default;

	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render() override;

	void ResetParam() override;
};