#pragma once
#include <memory>

#include "Actor.h"
#include "StatusData.h"

class CommandBase;
class BattleActorManager;
class Item;

class BattleActor : public Actor
{
protected:
	std::unique_ptr<CommandBase> mCommand = nullptr;
	Status mStatus;
	Item* mInventory;

public:
	BattleActor(const Actor* actor, Status status);
	virtual ~BattleActor() = default;

	virtual void Initialize() = 0;
	virtual bool Update(const BattleActorManager* bam);
	void RenderCommand() const;

	CommandBase* GetCommand() { return mCommand.get(); }
	Status* GetStatus() { return &mStatus; }
	Item* GetInventory() const { return mInventory; }

	bool IsDead() const { return mStatus.hp <= 0; }
};