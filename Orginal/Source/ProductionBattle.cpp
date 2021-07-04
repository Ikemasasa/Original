#include "ProductionBattle.h"

#include "lib/Math.h"

#include "BattleActorManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Define.h"
#include "EffectManager.h"
#include "GameManager.h"

ProductionBattle::ProductionBattle()
{
	mDeathEffectSlot = Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/Death/death.efk");
}

void ProductionBattle::Begin(CommandBase::Behaviour behaviour, int moveID, int targetID, int amount)
{
	mData.behaviour = behaviour;
	mData.moveID = moveID;
	mData.targetID = targetID;
	mData.amount = amount;
	mData.isEnable = true;
}

void ProductionBattle::Initialize()
{
	mProductionValue.Initialize();
}

bool ProductionBattle::Update(const BattleActorManager* bam)
{
	if (!mData.isEnable) return false;
	
	BattleActor* moveActor = bam->GetActor(mData.moveID);
	BattleActor* targetActor = bam->GetActor(mData.targetID);



	// •¶š(”š)‚Ì‰‰o‚Ìupdate
	mProductionValue.Update();

	return false;
}

void ProductionBattle::Render()
{
	mProductionValue.Render();
}
