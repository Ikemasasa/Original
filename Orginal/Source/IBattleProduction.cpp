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

// �����̐F
const Vector3 IBattleProduction::DAMAGE_RGB = { 1.0f, 0.4f, 0.4f };
const Vector3 IBattleProduction::HEAL_HP_RGB = { 0.76f, 0.85f, 0.25f };
const Vector3 IBattleProduction::HEAL_MP_RGB = { 0.54f, 0.32f, 0.63f };

bool IBattleProduction::StateWait(const float& waitTime)
{
	// �҂����Ԍo�߂��Ă���
	if (mWaitTimer >= waitTime)
	{
		// �����\�����Ȃ�false
		if(mProductionValue.IsProcessing()) return false;

		for (auto& target : mTargetCharas)
		{
			// ���S���[�V�������̃L����������Ȃ� false;
			if (target->GetExist() && target->GetMotion() == Character::DIE) return false;
		}
		
		// ���o�I���t���O�𗧂Ă�
		mIsFinished = true;

		return true;
	}

	// �҂����ԉ��Z
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
	// �^�[�Q�b�g�L�����S�{��
	for (auto& target : mTargetCharas)
	{
		// ���łɏ������I����Ă���continue
		if (!target->GetExist()) continue;

		// ���S���[�V�������A���[�V�������I������Ƃ���Ȃ�
		if (target->GetMotion() == Character::DIE && target->IsMotionFinished())
		{
			// �^�[�Q�b�g���\����
			target->SetExist(false);

			// �T�E���h�Đ�
			Sound::Play(Sound::DEATH);

			// �G�t�F�N�g�Đ�
			Vector3 pos = target->GetPos();
			Vector3 effectPos(pos.x, pos.y + target->GetLocalAABB().max.y * 0.5f, pos.z);

			EffectData::Param param = EffectData::GetParam(EffectData::DEATH);
			Singleton<EffectManager>().GetInstance().Play(param.slotID, effectPos, 0, param.scale, param.speed); // ���S�G�t�F�N�g
		}
	}
}

void IBattleProduction::ValueProduction()
{
	mProductionValue.Update();
}

void IBattleProduction::HitProduction(const SPHERE& sphere, const EffectData::Kind& effect, const Sound::Kind& sound)
{
	// �S�^�[�Q�b�g�L�����{��
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		BattleCharacter* target = mTargetCharas[i];

		// ���łɓ������Ă��邩�`�F�b�N
		bool ishit = false;
		for (auto& hit : mHitChara) if (hit == target) ishit = true;
		if (ishit) continue;

		// �����蔻��
		if (Collision::ColSphereCapsule(sphere, target->GetCapsule()))
		{
			// �q�b�g�L�����ɓo�^(�ȍ~������Ȃ��悤�ɂ��邽��
			mHitChara.emplace_back(target);

			// �q�b�g�T�E���h�Đ�
			Sound::Play(sound);

			// �q�b�g�G�t�F�N�g�Đ�
			EffectData::Param param = EffectData::GetParam(effect);
			int efcHandle = Singleton<EffectManager>().GetInstance().Play(param.slotID, target->GetTargetPos(), 0, param.scale, param.speed);
			mEffectInstHandles.push_back(efcHandle);

			// �J������h�炷
			Singleton<CameraManager>().GetInstance().Shake(Define::DAMAGE_SHAKE_DURATION, Define::DAMAGE_SHAKE_STRENGTH);

			// �K�[�h������Ȃ��Ȃ�_���[�W���[�V����
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
	// �S�^�[�Q�b�g�L�����{��
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		BattleCharacter* target = mTargetCharas[i];

		// �q�b�g�T�E���h�Đ�
		Sound::Play(sound);

		// �q�b�g�G�t�F�N�g�Đ�
		EffectData::Param param = EffectData::GetParam(effect);
		int efcHandle = Singleton<EffectManager>().GetInstance().Play(param.slotID, target->GetPos(), 0, param.scale, param.speed);
		mEffectInstHandles.push_back(efcHandle);


		if (mHPAmounts.size() > 0) mProductionValue.Add(mHPAmounts[i], target->GetTargetPos(), HEAL_HP_RGB);
		if (mMPAmounts.size() > 0) mProductionValue.Add(mMPAmounts[i], target->GetTargetPos(), HEAL_MP_RGB);
	}
}

void IBattleProduction::BuffProduction(const EffectData::Kind& effect, const Sound::Kind& sound)
{
	// �S�^�[�Q�b�g�L�����{��
	for (const auto& target : mTargetCharas)
	{
		// �T�E���h�Đ�
		Sound::Play(sound);

		// �G�t�F�N�g�Đ�
		EffectData::Param param = EffectData::GetParam(effect);
		int handle = Singleton<EffectManager>().GetInstance().Play(param.slotID, target->GetPos(), 0, param.scale, param.speed);
		mEffectInstHandles.push_back(handle);
	}
}

void IBattleProduction::AtkSkillProduction(const EffectData::Kind& effect, const Sound::Kind& sound)
{
	// �S�^�[�Q�b�g�L�����{��
	Vector3 total = {};
	int num = 0;
	for (const auto& target : mTargetCharas)
	{
		// ���ϒl���Ƃ�
		total += target->GetTargetPos();
		++num;
	}

	// �T�E���h�Đ�
	Sound::Play(sound);

	// �G�t�F�N�g�Đ�
	EffectData::Param param = EffectData::GetParam(effect);
	int handle = Singleton<EffectManager>().GetInstance().Play(param.slotID, total / num, 0, param.scale, param.speed);
	mEffectInstHandles.push_back(handle);
}
