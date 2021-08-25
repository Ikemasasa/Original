#pragma once
#include "IDecideCommand.h"

#include "ItemSelect.h"

class DecideItem : public IDecideCommand
{
	ItemSelect mItemSelect;

public:
	DecideItem() = default;
	~DecideItem() = default;

	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render() override;
};