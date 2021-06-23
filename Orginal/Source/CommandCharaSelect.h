#pragma once
#include "IDecideCommand.h"
#include "Actor.h"

class BattleActor;
class Sprite;

class CommandCharaSelect : public IDecideCommand
{
	static constexpr float ARROW_SCALE = 0.5f;

	std::unique_ptr<Sprite> mArrow;

	int mCharaIndex = 0;
	BattleActor* mTargetActor;
	Actor::Type mCharaType;

public:
	CommandCharaSelect(Actor::Type characterType);
	~CommandCharaSelect() = default;

	void Update(const BattleActorManager * bam, CommandBase * cmdBase);
	void Render() const;
};