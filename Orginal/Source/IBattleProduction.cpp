#include "IBattleProduction.h"

#include "lib/CollisionStructs.h"

#include "BattleCharacter.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Define.h"
#include "EffectData.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Singleton.h"

// 文字の色
const Vector3 IBattleProduction::DAMAGE_RGB = { 1.0f, 0.4f, 0.4f };
const Vector3 IBattleProduction::HEAL_HP_RGB = { 0.76f, 0.85f, 0.25f };
const Vector3 IBattleProduction::HEAL_MP_RGB = { 0.54f, 0.32f, 0.63f };

bool IBattleProduction::StateWait(const float& waitTime)
{
	// 待ち時間経過してたら
	if (mWaitTimer >= waitTime)
	{
		// 文字表示中ならfalse
		if(mProductionValue.IsProcessing()) return false;

		for (auto& target : mTargetCharas)
		{
			// 死亡モーション中のキャラがいるなら false;
			if (target->GetExist() && target->GetMotion() == Character::DIE) return false;
		}
		
		// 演出終了フラグを立てる
		mIsFinished = true;

		return true;
	}

	// 待ち時間加算
	mWaitTimer += GameManager::elapsedTime;
	return false;
}

void IBattleProduction::Begin(BattleCharacter* moveChara, std::vector<BattleCharacter*>& targetCharas)
{
	mMoveChara = moveChara;
	mTargetCharas = targetCharas;
}

void IBattleProduction::DeathProduction()
{
	// ターゲットキャラ全捜査
	for (auto& target : mTargetCharas)
	{
		// すでに処理が終わってたらcontinue
		if (!target->GetExist()) continue;

		// 死亡モーションかつ、モーションが終わったところなら
		if (target->GetMotion() == Character::DIE && target->IsMotionFinished())
		{
			// ターゲットを非表示に
			target->SetExist(false);

			// サウンド再生
			Sound::Play(Sound::DEATH);

			// エフェクト再生
			Vector3 pos = target->GetPos();
			Vector3 effectPos(pos.x, pos.y + target->GetLocalAABB().max.y * 0.5f, pos.z);

			EffectData::Param param = EffectData::GetParam(EffectData::DEATH);
			Singleton<EffectManager>().GetInstance().Play(param.slotID, effectPos, 0, param.scale, param.speed); // 死亡エフェクト
		}
	}
}

void IBattleProduction::ValueProduction()
{
	mProductionValue.Update();
}

void IBattleProduction::HitProduction(const SPHERE& sphere, const EffectData::Kind& effect, const Sound::Kind& sound)
{
	// 全ターゲットキャラ捜査
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		BattleCharacter* target = mTargetCharas[i];

		// すでに当たっているかチェック
		bool ishit = false;
		for (auto& hit : mHitChara) if (hit == target) ishit = true;
		if (ishit) continue;

		// 当たり判定
		if (Collision::ColSphereCapsule(sphere, target->GetCapsule()))
		{
			// ヒットキャラに登録(以降当たらないようにするため
			mHitChara.emplace_back(target);

			// ヒットサウンド再生
			Sound::Play(sound);

			// ヒットエフェクト再生
			EffectData::Param param = EffectData::GetParam(effect);
			int efcHandle = Singleton<EffectManager>().GetInstance().Play(param.slotID, target->GetTargetPos(), 0, param.scale, param.speed);
			mEffectInstHandles.push_back(efcHandle);

			// カメラを揺らす
			Singleton<CameraManager>().GetInstance().Shake(Define::DAMAGE_SHAKE_DURATION, Define::DAMAGE_SHAKE_STRENGTH);

			// ガード中じゃないならダメージモーション
			if (target->GetStatus()->IsDead())
			{
				target->SetMotionStopLastFrame(Character::DIE);
			}
			else
			{
				if (!target->GetStatus()->GetGuardFlag())
				{
					target->SetMotionOnce(Character::DAMAGE, Character::IDLE);
				}
			}

			if (mHPAmounts.size() > 0) mProductionValue.Add(mHPAmounts[i], target->GetTargetPos(), DAMAGE_RGB);
			if (mMPAmounts.size() > 0) mProductionValue.Add(mMPAmounts[i], target->GetTargetPos(), DAMAGE_RGB);
		}
	}
}

void IBattleProduction::HealProduction(const EffectData::Kind& effect, const Sound::Kind& sound)
{
	// 全ターゲットキャラ捜査
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		BattleCharacter* target = mTargetCharas[i];

		// ヒットサウンド再生
		Sound::Play(sound);

		// ヒットエフェクト再生
		EffectData::Param param = EffectData::GetParam(effect);
		int efcHandle = Singleton<EffectManager>().GetInstance().Play(param.slotID, target->GetPos(), 0, param.scale, param.speed);
		mEffectInstHandles.push_back(efcHandle);


		if (mHPAmounts.size() > 0) mProductionValue.Add(mHPAmounts[i], target->GetTargetPos(), HEAL_HP_RGB);
		if (mMPAmounts.size() > 0) mProductionValue.Add(mMPAmounts[i], target->GetTargetPos(), HEAL_MP_RGB);
	}
}

void IBattleProduction::BuffProduction(const EffectData::Kind& effect, const Sound::Kind& sound)
{
	// 全ターゲットキャラ捜査
	for (const auto& target : mTargetCharas)
	{
		// サウンド再生
		Sound::Play(sound);

		// エフェクト再生
		EffectData::Param param = EffectData::GetParam(effect);
		int handle = Singleton<EffectManager>().GetInstance().Play(param.slotID, target->GetPos(), 0, param.scale, param.speed);
		mEffectInstHandles.push_back(handle);
	}
}

void IBattleProduction::AtkSkillProduction(const EffectData::Kind& effect, const Sound::Kind& sound)
{
	// 全ターゲットキャラ捜査
	Vector3 total = {};
	int num = 0;
	for (const auto& target : mTargetCharas)
	{
		// 平均値をとる
		total += target->GetTargetPos();
		++num;
	}

	// サウンド再生
	Sound::Play(sound);

	// エフェクト再生
	EffectData::Param param = EffectData::GetParam(effect);
	int handle = Singleton<EffectManager>().GetInstance().Play(param.slotID, total / num, 0, param.scale, param.speed);
	mEffectInstHandles.push_back(handle);
}
