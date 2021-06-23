#include "ProductionBattle.h"

#include "lib/Math.h"

#include "BattleActorManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Define.h"
#include "FrontendBattle.h"
#include "GameManager.h"

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
	
	BattleActor* moveActor = bam->GetActor(mData.moveID);
	BattleActor* targetActor = bam->GetActor(mData.targetID);

	auto Init = [&]()
	{
		const float ADJUST_DIST_MUL = 2;

		mOrgPos = moveActor->GetPos();

		Vector3 targetToOrg = mOrgPos - targetActor->GetPos();
		if (targetToOrg == Vector3::Zero()) targetToOrg = Vector3(sinf(targetActor->GetAngle().y), targetActor->GetPos().y, cosf(targetActor->GetAngle().y));
		else							    targetToOrg.Normalize();

		mDestinationPos = targetActor->GetPos() + targetToOrg * targetActor->GetCapsule().radius * ADJUST_DIST_MUL;

		++mState;
	};
	auto MoveToTarget = [&]()
	{
		moveActor->SetMotion(SkinnedMesh::RUN);

		const float LERPFACTOR_MAX = 1.0f;
		mLerpFactor = Math::Min(mLerpFactor + LERP_FACTOR_ADD, LERPFACTOR_MAX);
		Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
		moveActor->SetPos(lerp);

		if (mLerpFactor >= LERPFACTOR_MAX)
		{
			moveActor->SetMotion(SkinnedMesh::ATTACK, false);
			++mState;
		}
	};
	auto MoveToOrigin = [&]()
	{
		moveActor->SetMotion(SkinnedMesh::RUN);

		BattleActor* targetActor = bam->GetActor(mData.targetID);

		const float LERPFACTOR_MIN = 0.0f;
		mLerpFactor = Math::Max(mLerpFactor - LERP_FACTOR_ADD, LERPFACTOR_MIN);
		Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
		moveActor->SetPos(lerp);

		if (mLerpFactor <= 0.0f)
		{
			moveActor->SetMotion(SkinnedMesh::IDLE);
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
		if (moveActor->IsMotionFinished())
		{
			Vector3 damagePos(targetActor->GetPos().x, targetActor->GetAABB().max.y, targetActor->GetPos().z);
			Vector4 damageCol(1.0f, 0.4f, 0.4f, 0.0f);
			//FrontendBattle::GetInstance().SetValue(mData.amount, damagePos, Vector2(0.5f, 0.0f), damageCol);
			++mState;
		}
		break;

	case 3: // ˆÚ“®i‚à‚Ç‚éj
		MoveToOrigin();
		break;

	case 4: // ‚¿‚å‚Á‚Æ‘Ò‹@ŽžŠÔ(1•b‚­‚ç‚¢)
		mTimer += GameManager::elpsedTime;

		const float WAIT_SEC = 1.5f;
		if (mTimer >= WAIT_SEC)
		{
			mState = 0;
			mTimer = 0.0f;
			mData.isEnable = false;
			return true;
		}
	}

	return false;
}
