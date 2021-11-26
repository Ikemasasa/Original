#include "ProductionAttack.h"

#include "lib/SkinnedMesh.h"

#include "BattleCharacter.h"
#include "BattleCharacterManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "DamageCalculator.h"
#include "Define.h"
#include "EffectData.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Singleton.h"
#include "StatusData.h"
#include "Sound.h"

void ProductionAttack::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionAttack::Update()
{
	switch (mState)
	{
	case State::INIT: // 初期化 ダメージ計算

		// ダメージ計算
		for (size_t i = 0; i < mTargetCharas.size(); ++i)
		{
			int damage = DamageCalculator::CalcAttackDamage(mMoveChara->GetStatus(), mTargetCharas[i]->GetStatus());
			mTargetCharas[i]->GetStatus()->SubHP(damage);
			mHPAmounts.push_back(damage);
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
	targetPos /= static_cast<float>(mTargetCharas.size());
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
	mMoveChara->SetMotion(Character::RUN);

	// mOrgPos から mDestinationPos までを線形補完する
	mLerpFactor = Math::Min(mLerpFactor + LERP_FACTOR_ADD, LERP_FACTOR_MAX);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveChara->SetPos(lerp);

	// mLerpFactorが1以上なら
	if (mLerpFactor >= LERP_FACTOR_MAX)
	{
		// 攻撃モーションセット
		mMoveChara->SetMotionOnce(Character::ATTACK, Character::RUN);
		++mState;
	}
}

void ProductionAttack::StateWaitAttack()
{
	// 攻撃キャラの当たり判定が有効なら
	if (mMoveChara->IsBoneColEnable())
	{
		Matrix bone = {};
		float radius = 0.0f;

		// パラメータ取得
		mMoveChara->GetBoneCollisionParam(&bone, &radius);

		// ヒット演出のパラメータ設定
		Sound::Kind sound = Sound::ATTACK_HIT;
		EffectData::Kind effect = EffectData::DAMAGE;
		SPHERE sphere = { Vector3(bone._41, bone._42, bone._43), radius };

		// ヒット演出
		IBattleProduction::HitProduction(sphere, effect, sound);
	}

	// モーションが終わったら
	if (mMoveChara->IsMotionFinished())
	{		
		// 元居た方向に向く
		mMoveChara->SetAngleY(mMoveChara->GetAngle().y + Define::PI); // 向きを反転
		++mState;
	}
}

void ProductionAttack::StateMoveToOrigin()
{
	// 走りモーションセット
	mMoveChara->SetMotion(Character::RUN);

	// mDestinationPos から mOrgPos までを線形補完する
	mLerpFactor = Math::Max(mLerpFactor - LERP_FACTOR_ADD, LERP_FACTOR_MIN);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveChara->SetPos(lerp);

	// mLerpFactorが0以下なら
	if (mLerpFactor <= LERP_FACTOR_MIN)
	{
		// 待機モーションセット、Angleを敵方向に戻す
		mMoveChara->SetMotion(Character::IDLE);
		mMoveChara->SetAngleY(mMoveChara->GetAngle().y + Define::PI); // 向きを反転
		++mState;
	}
}