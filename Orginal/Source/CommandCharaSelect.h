#pragma once
#include "IDecideCommand.h"

#include "Character.h"

class BattleCharacter;
class BattleCharacterManager;
class CommandBase;
class Sprite;

class CommandCharaSelect : public IDecideCommand
{
	static constexpr float ARROW_SCALE = 0.5f;

	std::unique_ptr<Sprite> mArrow;

	int mCharaIndex = 0;
	BattleCharacter* mTargetChara;
	Character::Type mCharaType;

public:
	CommandCharaSelect(Character::Type characterType);
	~CommandCharaSelect() = default;

	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render() override;
};