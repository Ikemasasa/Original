#include "ProductionAttack.h"

#include "BattleActor.h"
#include "Define.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "SkinnedMesh.h"
#include "Singleton.h"

void ProductionAttack::Initialize()
{
	Singleton<EffectManager>().GetInstance().Create
	mProductionValue.Initialize();
}

int ProductionAttack::Update()
{
	int ret = -1;

	auto MoveToOrigin = [&]()
	{
		// 走りモーションセット
		mMoveActor->SetMotion(SkinnedMesh::RUN);

		// mDestinationPos から mOrgPos  までを線形補完する
		mLerpFactor = Math::Max(mLerpFactor - LERP_FACTOR_ADD, LERP_FACTOR_MIN);
		Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
		mMoveActor->SetPos(lerp);

		// mLerpFactorが0以下なら
		if (mLerpFactor <= LERP_FACTOR_MIN)
		{
			// 待機モーションセット、Angleを敵方向に戻す
			mMoveActor->SetMotion(SkinnedMesh::IDLE);
			mMoveActor->SetAngleY(mMoveActor->GetAngle().y + Define::PI); // 向きを反転
			++mState;
		}
	};

	switch (mState)
	{
	case State::INIT: // 初期化
		StateInit();
		//break;

	case State::MOVE_TO_TARGET: // 移動
		StateMoveToTarget();
		break;

	case State::WAIT_ATTACK: // 攻撃終了待ち
		StateWaitAttack();
		break;

	case State::MOVE_TO_ORIGIN: // 移動（もどる）
		StateMoveToOrigin();
		break;

	case State::WAIT: // ちょっと待機時間(1秒くらい)
		mWaitTimer += GameManager::elpsedTime;

		const float WAIT_SEC = 1.5f;
		if (mWaitTimer >= WAIT_SEC)
		{
			mState = 0;
			mWaitTimer = 0.0f;
		}
	}


}

void ProductionAttack::Render()
{
	mProductionValue.Render();
}


void ProductionAttack::StateInit()
{
	// MoveActorの座標を原点にする
	mOrgPos = mMoveActor->GetPos();

	// TargetActor から MoveActor のベクトルを作って、行き先を決める(mDestinationPos)
	const float ADJUST_DIST_MUL = 2;
	Vector3 targetToOrg = mOrgPos - mTargetActor->GetPos();
	targetToOrg.Normalize();
	//if (targetToOrg == Vector3::Zero()) targetToOrg = Vector3(sinf(targetActor->GetAngle().y), targetActor->GetPos().y, cosf(targetActor->GetAngle().y));
	//else							    

	mDestinationPos = mTargetActor->GetPos() + targetToOrg * mTargetActor->GetCapsule().radius * ADJUST_DIST_MUL;

	++mState; // ステートを次に進める
}

void ProductionAttack::StateMoveToTarget()
{
	// 走りモーションセット
	mMoveActor->SetMotion(SkinnedMesh::RUN);

	// mOrgPos から mDestinationPos までを線形補完する
	mLerpFactor = Math::Min(mLerpFactor + LERP_FACTOR_ADD, LERP_FACTOR_MAX);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveActor->SetPos(lerp);

	// mLerpFactorが1以上なら
	if (mLerpFactor >= LERP_FACTOR_MAX)
	{
		// 攻撃モーションセット
		mMoveActor->SetMotion(SkinnedMesh::ATTACK, false);
		++mState;
	}
}

void ProductionAttack::StateWaitAttack()
{
	// モーションが終わったら
	if (mMoveActor->IsMotionFinished())
	{
		if (mTargetActor->GetStatus()->IsDead()) // 攻撃対象が死んでたら
		{
			// 見えなくして、エフェクト再生
			mTargetActor->SetExist(false); // 見えなくする
			Vector3 effectPos(mTargetActor->GetPos().x, mTargetActor->GetPos().y + mTargetActor->GetLocalAABB().max.y * 0.5f, mTargetActor->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(mDeathEffectSlot, effectPos);// えっふぇくと
		}
		Vector3 damagePos(mTargetActor->GetPos().x, mTargetActor->GetAABB().max.y, mTargetActor->GetPos().z);
		Vector3 damageRGB(1.0f, 0.4f, 0.4f);
		Vector2 center(0.0f, 0.0f);
		Vector2 scale(1.0f, 1.0f);
		mProductionValue.Add(mData.amount, damagePos, damageRGB);

		Vector3 dist = mOrgPos - mDestinationPos; // 向きたい方向ベクトル
		dist.Normalize();
		moveActor->CorrectionAngle(dist);
		++mState;
	}
}

void ProductionAttack::StateMoveToOrigin()
{

}



void ProductionAttack::StateWait()
{
}