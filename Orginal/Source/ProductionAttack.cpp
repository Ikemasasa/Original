#include "ProductionAttack.h"

#include "lib/SkinnedMesh.h"

#include "BattleActor.h"
#include "BattleActorManager.h"
#include "Define.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Singleton.h"
#include "StatusData.h"

void ProductionAttack::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionAttack::Update(const BattleActorManager* bam)
{
	switch (mState)
	{
	case State::INIT: // 初期化 ダメージ計算
	{
		// moveactor, targetactor代入
		mMoveActor = bam->GetActor(mMoveActorID);
		mTargetActor = bam->GetActor(mTargetActorID);

		// ダメージ計算
		int damage = CalcDamage(mMoveActor->GetStatus(), mTargetActor->GetStatus());
		mTargetActor->GetStatus()->HurtHP(damage);
		mAmount = damage;
	}
		// 演出初期設定
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
		StateWait();
		break;
	}

	// ダメージ演出(ダメージ量が表示される)のupdate
	mProductionValue.Update();

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
	Vector3 targetToOrg = mOrgPos - mTargetActor->GetPos();
	targetToOrg.Normalize();

	const float ADJUST_DIST_MUL = 4;
	mDestinationPos = mTargetActor->GetPos() + targetToOrg * mTargetActor->GetCapsule().radius * ADJUST_DIST_MUL;

	// 行く方向に向く
	mMoveActor->CorrectionAngle(-targetToOrg);

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
		if (mTargetActor->GetStatus()->IsDead())
		{
			// Existをfalseにして、エフェクトを再生する
			mTargetActor->SetExist(false); // 見えなくする
			Vector3 effectPos(mTargetActor->GetPos().x, mTargetActor->GetPos().y + mTargetActor->GetLocalAABB().max.y * 0.5f, mTargetActor->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// えっふぇくと
		}

		Vector3 damagePos(mTargetActor->GetPos().x, mTargetActor->GetAABB().max.y, mTargetActor->GetPos().z);
		mProductionValue.Add(mAmount, damagePos, DAMAGE_RGB);

		// 元居た方向に向く
		mMoveActor->SetAngleY(mMoveActor->GetAngle().y + Define::PI); // 向きを反転
		++mState;
	}
}

void ProductionAttack::StateMoveToOrigin()
{
	// 走りモーションセット
	mMoveActor->SetMotion(SkinnedMesh::RUN);

	// mDestinationPos から mOrgPos までを線形補完する
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
}



void ProductionAttack::StateWait()
{
	mWaitTimer += GameManager::elpsedTime;

	const float WAIT_SEC = 1.5f;
	if (mWaitTimer >= WAIT_SEC)
	{
		mState = 0;
		mWaitTimer = 0.0f;
		mIsFinished = true;
	}
}

int ProductionAttack::CalcDamage(const Status* deal, Status* take)
{
	const int STR_DIV = 2;
	const int VIT_DIV = 4;
	const int WIDTH_DIV = 4;

	int damage = deal->str / STR_DIV - take->vit / VIT_DIV; // 基礎ダメージ
	int sign = (rand() % 2 == 0) ? -1 : 1; // 振れ幅の符号
	int width = damage / WIDTH_DIV + 1; // ダメージの振れ幅の最大値
	damage = damage + (rand() % width * sign);

	if (damage < 0) damage = 0;

	return damage;
}
