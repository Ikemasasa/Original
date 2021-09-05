#pragma once
#include <memory>

#include "Character.h"
#include "Item.h"
#include "StatusData.h"

class CommandBase;
class BattleCharacterManager;

class BattleCharacter : public Character
{
protected:
	std::unique_ptr<CommandBase> mCommand = nullptr;
	Status mStatus;
	Item mInventory;

public:
	BattleCharacter(const Character* chara, Status status);
	virtual ~BattleCharacter() = default;

	virtual void Initialize() = 0;
	virtual void Update(const BattleCharacterManager* bcm);
	void RenderCommand() const;

	CommandBase* GetCommand() { return mCommand.get(); }
	Status* GetStatus() { return &mStatus; }
	Item* GetInventory() { return &mInventory; }
};