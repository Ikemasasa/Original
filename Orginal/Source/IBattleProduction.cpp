#include "IBattleProduction.h"

#include "BattleCharacter.h"
#include "Collision.h"
#include "EffectManager.h"
#include "Singleton.h"
#include "TurnManager.h"

// •¶Žš‚ÌF
const Vector3 IBattleProduction::DAMAGE_RGB = { 1.0f, 0.4f, 0.4f };
const Vector3 IBattleProduction::HEAL_HP_RGB = { 0.76f, 0.85f, 0.25f };
const Vector3 IBattleProduction::HEAL_MP_RGB = { 0.54f, 0.32f, 0.63f };

void IBattleProduction::UpdateDeathMotion()
{
	for (auto& target : mTargetCharas)
	{
		if (!target->GetExist()) continue;

		if (target->GetMotion() == Character::DIE)
		{
			if (target->IsMotionFinished())
			{
				target->SetExist(false); // Œ©‚¦‚È‚­‚·‚é
				Vector3 pos = target->GetPos();
				Vector3 effectPos(pos.x, pos.y + target->GetLocalAABB().max.y * 0.5f, pos.z);
				Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// ‚¦‚Á‚Ó‚¥‚­‚Æ
			}
		}
	}
}

void IBattleProduction::Begin(const int moveCharaID, const std::vector<int>& targetCharaIDs)
{
	mMoveCharaID = moveCharaID;
	mTargetCharaIDs = targetCharaIDs;
}