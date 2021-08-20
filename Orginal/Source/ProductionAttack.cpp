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
	mAmounts.clear();
}

void ProductionAttack::Update(const BattleCharacterManager* bcm)
{
	switch (mState)
	{
	case State::INIT: // 初期化 ダメージ計算
	{
		// moveChara, targetChara代入
		mMoveChara = bcm->GetChara(mMoveCharaID);
		for (size_t i = 0; i < mTargetCharaIDs.size(); ++i)
		{
			mTargetCharas.push_back(bcm->GetChara(mTargetCharaIDs[i]));
		}

		// ダメージ計算
		for (size_t i = 0; i < mTargetCharas.size(); ++i)
		{
			int damage = DamageCalculator::CalcAttackDamage(mMoveChara->GetStatus(), mTargetCharas[i]->GetStatus());
			mTargetCharas[i]->GetStatus()->SubHP(damage);
			mAmounts.push_back(damage);
		}
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
	// ターゲットの平均をとる
	Vector3 targetPos = {};
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		targetPos += mTargetCharas[i]->GetPos();
	}
	targetPos /= mTargetCharas.size();
	Vector3 orgToTarget = targetPos - mOrgPos;
	Vector3 N = orgToTarget.GetNormalize();

	const float ADJUST_MUL = 3;
	orgToTarget += -(N * mMoveChara->GetCapsule().radius * ADJUST_MUL);
	mDestinationPos = mOrgPos + orgToTarget;

	// 行く方向に向く
	mMoveChara->CorrectionAngle(N);

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
		for (int i = 0; i < mTargetCharas.size(); ++i)
		{
			if (mTargetCharas[i]->GetStatus()->IsDead())
			{
				// Existをfalseにして、エフェクトを再生する
				mTargetCharas[i]->SetExist(false); // 見えなくする
				Vector3 effectPos(mTargetCharas[i]->GetPos().x, mTargetCharas[i]->GetPos().y + mTargetCharas[i]->GetLocalAABB().max.y * 0.5f, mTargetCharas[i]->GetPos().z);
				Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// えっふぇくと
			}

			Vector3 damagePos(mTargetCharas[i]->GetPos().x, mTargetCharas[i]->GetAABB().max.y, mTargetCharas[i]->GetPos().z);
			mProductionValue.Add(mAmounts[i], damagePos, DAMAGE_RGB);
		}
		
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