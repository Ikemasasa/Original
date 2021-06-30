#include "ProductionBattle.h"

#include "lib/Math.h"

#include "BattleActorManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Define.h"
#include "EffectManager.h"
#include "FrontendBattle.h"
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
			moveActor->SetAngleY(moveActor->GetAngle().y + Define::PI); // 向きを戻す
			++mState;
		}
	};

	switch (mState)
	{
	case 0: // 初期化
		Init();
		//break;

	case 1: // 移動
		MoveToTarget();

		break;

	case 2: // 攻撃終了待ち
		if (moveActor->IsMotionFinished())
		{
			if (targetActor->GetStatus()->IsDead()) // 攻撃対象が死んでたら
			{
				targetActor->SetExist(false); // 見えなくする
				Vector3 effectPos(targetActor->GetPos().x, targetActor->GetPos().y + targetActor->GetLocalAABB().max.y * 0.5f, targetActor->GetPos().z);
				Singleton<EffectManager>().GetInstance().Play(mDeathEffectSlot, effectPos);// えっふぇくと
			}
			Vector3 damagePos(targetActor->GetPos().x, targetActor->GetAABB().max.y, targetActor->GetPos().z);
			Vector3 damageRGB(1.0f, 0.4f, 0.4f);
			Vector2 center(0.0f, 0.0f);
			Vector2 scale(1.0f, 1.0f);
			mProductionValue.Add(mData.amount, damagePos, damageRGB);

			Vector3 dist = mOrgPos - mDestinationPos; // 向きたい方向ベクトル
			dist.Normalize();
			moveActor->CorrectionAngle(dist);
			++mState;
		}
		break;

	case 3: // 移動（もどる）
		MoveToOrigin();
		break;

	case 4: // ちょっと待機時間(1秒くらい)
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

	// 文字(数字)の演出のupdate
	mProductionValue.Update();

	return false;
}

void ProductionBattle::Render()
{
	mProductionValue.Render();
}
