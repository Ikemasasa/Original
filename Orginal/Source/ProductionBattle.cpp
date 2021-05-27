#include "ProductionBattle.h"

#include "lib/Math.h"

#include "BattleActorManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Define.h"
#include "FrontendBattle.h"

void ProductionBattle::Begin(CommandBase::Behaviour behaviour, int moveID, int targetID, int amount)
{
	mData.behaviour = behaviour;
	mData.moveID = moveID;
	mData.targetID = targetID;
	mData.amount = amount;
	mData.isEnable = true;
}

bool ProductionBattle::Update(const BattleActorManager* bam)
{
	if (!mData.isEnable) return false;
	
	std::shared_ptr<BattleActor> moveActor = bam->GetActor(mData.moveID);
	std::shared_ptr<BattleActor> targetActor = bam->GetActor(mData.targetID);

	auto Init = [&]()
	{
		mOrgPos = moveActor->GetPos();

		// ˆÚ“®æ‚ÌÀ•W‚ðŒvŽZ
		mDestinationPos = targetActor->GetPos();
		++mState;
	};
	auto MoveToTarget = [&]()
	{
		moveActor->SetMotion(SkinnedMesh::RUN);

		mLerpFactor += 0.02f;
		moveActor->SetPos(Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor));


		if (mLerpFactor >= 0.8f)
		{
			moveActor->SetMotion(SkinnedMesh::ATTACK, false);
			++mState;
		}
	};
	auto MoveToOrigin = [&]()
	{
		std::shared_ptr<BattleActor> targetActor = bam->GetActor(mData.targetID);

		mLerpFactor = Math::Max(0.0f, mLerpFactor - 0.02f);
		moveActor->SetPos(Vector3::Lerp(mOrgPos, targetActor->GetPos(), mLerpFactor));

		if (mLerpFactor <= 0.0f)
		{
			++mState;
		}
	};

	switch (mState)
	{
	case 0: // ‰Šú‰»
		Init();
		//break;

	case 1: // ˆÚ“®
		MoveToTarget();

		break;

	case 2: // UŒ‚I—¹‘Ò‚¿
		if (moveActor->GetMotion() == SkinnedMesh::IDLE)
		{
			Vector3 damagePos(targetActor->GetPos().x, targetActor->GetAABB().max.y, targetActor->GetPos().z);
			Vector4 damageCol(1.0f, 0.4f, 0.4f, 0.0f);
			FrontendBattle::GetInstance().SetValue(mData.amount, damagePos, Vector2(0.5f, 0.0f), damageCol);
			++mState;
		}
		break;

	case 3: // ˆÚ“®i‚à‚Ç‚éj
		MoveToOrigin();
		break;

	case 4: // ‚¿‚å‚Á‚Æ‘Ò‹@ŽžŠÔ(1•b‚­‚ç‚¢)
		mTimer += 1.0f;

		const float WAIT_SEC = 1.5f;
		if (mTimer >= Define::FRAMERATE * WAIT_SEC)
		{
			mState = 0;
			mTimer = 0.0f;
			mData.isEnable = false;
			return true;
		}
	}

	return false;
}
