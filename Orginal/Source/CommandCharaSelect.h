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

	BattleCharacter* mTargetChara;
	Character::Type mCharaType;

	void SetBehaviourAttack(CommandBase* cmdBase);
	void SetBehaviourUseItem(CommandBase* cmdBase);
	void SetBehaviourUseSkill(CommandBase* cmdBase);

public:
	CommandCharaSelect(Character::Type characterType);
	~CommandCharaSelect() = default;

	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render() override;
};