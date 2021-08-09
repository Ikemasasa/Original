#include "ProductionAttack.h"

#include "lib/SkinnedMesh.h"

#include "BattleCharacter.h"
#include "BattleCharacterManager.h"
#include "DamageCalculator.h"
#include "Define.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Singleton.h"
#include "StatusData.h"


void ProductionAttack::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionAttack::Update(const BattleCharacterManager* bcm)
{
	switch (mState)
	{
	case State::INIT: // 初期化 ダメージ計算
	{
		// moveChara, targetChara代入
		mMoveChara = bcm->GetChara(mMoveCharaID);
		mTargetChara = bcm->GetChara(mTargetCharaID);

		// ダメージ計算
		int damage = DamageCalculator::CalcAttackDamage(mMoveChara->GetStatus(), mTargetChara->GetStatus());
		mTargetChara->GetStatus()->HurtHP(damage);
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
	// MoveCharaの座標を原点にする
	mOrgPos = mMoveChara->GetPos();

	// TargetChara から MoveChara のベクトルを作って、行き先を決める(mDestinationPos)
	Vector3 targetToOrg = mOrgPos - mTargetChara->GetPos();
	targetToOrg.Normalize();

	const float ADJUST_DIST_MUL = 4;
	mDestinationPos = mTargetChara->GetPos() + targetToOrg * mTargetChara->GetCapsule().radius * ADJUST_DIST_MUL;

	// 行く方向に向く
	mMoveChara->CorrectionAngle(-targetToOrg);

	++mState; // ステートを次に進める
}

void ProductionAttack::StateMoveToTarget()
{
	// 走りモーションセット
	mMoveChara->SetMotion(SkinnedMesh::RUN);

	// mOrgPos から mDestinationPos までを線形補完する
	mLerpFactor = Math::Min(mLerpFactor + LERP_FACTOR_ADD, LERP_FACTOR_MAX);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveChara->SetPos(lerp);

	// mLerpFactorが1以上なら
	if (mLerpFactor >= LERP_FACTOR_MAX)
	{
		// 攻撃モーションセット
		mMoveChara->SetMotion(SkinnedMesh::ATTACK, false);
		++mState;
	}
}

void ProductionAttack::StateWaitAttack()
{
	// モーションが終わったら
	if (mMoveChara->IsMotionFinished())
	{
		if (mTargetChara->GetStatus()->IsDead())
		{
			// Existをfalseにして、エフェクトを再生する
			mTargetChara->SetExist(false); // 見えなくする
			Vector3 effectPos(mTargetChara->GetPos().x, mTargetChara->GetPos().y + mTargetChara->GetLocalAABB().max.y * 0.5f, mTargetChara->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// えっふぇくと
		}

		Vector3 damagePos(mTargetChara->GetPos().x, mTargetChara->GetAABB().max.y, mTargetChara->GetPos().z);
		mProductionValue.Add(mAmount, damagePos, DAMAGE_RGB);

		// 元居た方向に向く
		mMoveChara->SetAngleY(mMoveChara->GetAngle().y + Define::PI); // 向きを反転
		++mState;
	}
}

void ProductionAttack::StateMoveToOrigin()
{
	// 走りモーションセット
	mMoveChara->SetMotion(SkinnedMesh::RUN);

	// mDestinationPos から mOrgPos までを線形補完する
	mLerpFactor = Math::Max(mLerpFactor - LERP_FACTOR_ADD, LERP_FACTOR_MIN);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveChara->SetPos(lerp);

	// mLerpFactorが0以下なら
	if (mLerpFactor <= LERP_FACTOR_MIN)
	{
		// 待機モーションセット、Angleを敵方向に戻す
		mMoveChara->SetMotion(SkinnedMesh::IDLE);
		mMoveChara->SetAngleY(mMoveChara->GetAngle().y + Define::PI); // 向きを反転
		++mState;
	}
}



void ProductionAttack::StateWait()
{
	mWaitTimer += GameManager::elapsedTime;

	const float WAIT_SEC = 1.5f;
	if (mWaitTimer >= WAIT_SEC)
	{
		mState = 0;
		mWaitTimer = 0.0f;
		mIsFinished = true;
	}
}