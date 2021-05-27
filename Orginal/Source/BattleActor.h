#pragma once
#include <memory>

#include "Actor.h"
#include "StatusData.h"

class CommandBase;
class BattleActorManager;

class BattleActor : public Actor
{
protected:
	std::unique_ptr<CommandBase> mCommand = nullptr;
	Status mStatus;

public:
	BattleActor(const std::shared_ptr<Actor>& actor, Status status);
	virtual ~BattleActor() = default;

	virtual void Initialize() = 0;
	virtual bool Update(const BattleActorManager* bam);


	CommandBase* GetCommand() { return mCommand.get(); }
	Status* GetStatus() { return &mStatus; }

	bool IsDead() const { return mStatus.hp <= 0; }
};