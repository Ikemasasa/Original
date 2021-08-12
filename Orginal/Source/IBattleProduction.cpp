#include "IBattleProduction.h"

// ï∂éöÇÃêF
const Vector3 IBattleProduction::DAMAGE_RGB = { 1.0f, 0.4f, 0.4f };
const Vector3 IBattleProduction::HEAL_HP_RGB = { 0.76f, 0.85f, 0.25f };
const Vector3 IBattleProduction::HEAL_MP_RGB = { 0.54f, 0.32f, 0.63f };

void IBattleProduction::Begin(const int moveCharaID, const std::vector<int>& targetCharaIDs)
{
	mMoveCharaID = moveCharaID;
	mTargetCharaIDs = targetCharaIDs;
}