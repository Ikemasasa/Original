#pragma once
#include "IBattleProduction.h"

class ProductionUseItem : public IBattleProduction
{
public:
	void Initialize();
	void Update(const BattleActorManager* bam);
	void Render();
};