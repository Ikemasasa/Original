#include "ProductionAttack.h"

#include "lib/Audio.h"
#include "lib/SkinnedMesh.h"

#include "BattleCharacter.h"
#include "BattleCharacterManager.h"
#include "Collision.h"
#include "DamageCalculator.h"
#include "Define.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Singleton.h"
#include "StatusData.h"
#include "TurnManager.h"

void ProductionAttack::Initialize()
{
	mProductionValue.Initialize();
	mAmounts.clear();
}

void ProductionAttack::Update(const BattleCharacterManager* bcm)
{
	switch (mState)
	{
	case State::INIT: // ������ �_���[�W�v�Z
	{
		// moveChara, targetChara���
		mMoveChara = bcm->GetChara(mMoveCharaID);
		for (size_t i = 0; i < mTargetCharaIDs.size(); ++i)
		{
			mTargetCharas.push_back(bcm->GetChara(mTargetCharaIDs[i]));
		}

		// �_���[�W�v�Z
		for (size_t i = 0; i < mTargetCharas.size(); ++i)
		{
			int damage = DamageCalculator::CalcAttackDamage(mMoveChara->GetStatus(), mTargetCharas[i]->GetStatus());
			mTargetCharas[i]->GetStatus()->SubHP(damage);
			mAmounts.push_back(damage);
		}
	}
		// ���o�����ݒ�
		StateInit();
		//break;

	case State::MOVE_TO_TARGET: // �ړ�
		StateMoveToTarget();
		break;

	case State::WAIT_ATTACK: // �U���I���҂�
		StateWaitAttack();
		break;

	case State::MOVE_TO_ORIGIN: // �ړ��i���ǂ�j
		StateMoveToOrigin();
		break;

	case State::WAIT: // ������Ƒҋ@����(1�b���炢)
		StateWait();
		break;
	}

	// �_���[�W���o(�_���[�W�ʂ��\�������)��update
	mProductionValue.Update();
}

void ProductionAttack::Render()
{
	mProductionValue.Render();
}


void ProductionAttack::StateInit()
{
	// MoveChara�̍��W�����_�ɂ���
	mOrgPos = mMoveChara->GetPos();

	// TargetChara ���� MoveChara �̃x�N�g��������āA�s��������߂�(mDestinationPos)
	// �^�[�Q�b�g�̕��ς��Ƃ�
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

	// �s�������Ɍ���
	mMoveChara->CorrectionAngle(N);

	++mState; // �X�e�[�g�����ɐi�߂�
}

void ProductionAttack::StateMoveToTarget()
{
	// ���胂�[�V�����Z�b�g
	mMoveChara->SetMotion(Character::RUN);

	// mOrgPos ���� mDestinationPos �܂ł���`�⊮����
	mLerpFactor = Math::Min(mLerpFactor + LERP_FACTOR_ADD, LERP_FACTOR_MAX);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveChara->SetPos(lerp);

	// mLerpFactor��1�ȏ�Ȃ�
	if (mLerpFactor >= LERP_FACTOR_MAX)
	{
		// �U�����[�V�����Z�b�g
		mMoveChara->SetMotionOnce(Character::ATTACK, Character::RUN);
		++mState;
	}
}

void ProductionAttack::StateWaitAttack()
{
	// �R���W����
	if (mMoveChara->IsBoneColEnable())
	{
		Matrix bm = {};
		float radius = 0.0f;
		mMoveChara->GetBoneCollisionParam(&bm, &radius);
		SPHERE sphere = { Vector3(bm._41, bm._42, bm._43), radius };
		for (size_t i = 0; i < mTargetCharas.size(); ++i)
		{
			BattleCharacter* target = mTargetCharas[i];

			// ���łɓ������Ă��邩�`�F�b�N
			bool ishit = false;
			for (auto& hit : mHitChara) if (hit == target) ishit = true;
			if (ishit) continue;

			if (Collision::ColSphereCapsule(sphere, target->GetCapsule()))
			{
				mHitChara.emplace_back(target);
				Audio::SoundPlay((int)Sound::ATTACK_HIT);

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

				Vector3 p = target->GetPos();
				Vector3 damagePos(p.x, mTargetCharas[i]->GetAABB().max.y, p.z);
				mProductionValue.Add(mAmounts[i], damagePos, DAMAGE_RGB);
			}
		}
	}

	// ���[�V�������I�������
	if (mMoveChara->IsMotionFinished())
	{		
		// �����������Ɍ���
		mMoveChara->SetAngleY(mMoveChara->GetAngle().y + Define::PI); // �����𔽓]
		++mState;
	}
}

void ProductionAttack::StateMoveToOrigin()
{
	// ���胂�[�V�����Z�b�g
	mMoveChara->SetMotion(Character::RUN);

	// mDestinationPos ���� mOrgPos �܂ł���`�⊮����
	mLerpFactor = Math::Max(mLerpFactor - LERP_FACTOR_ADD, LERP_FACTOR_MIN);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveChara->SetPos(lerp);

	// mLerpFactor��0�ȉ��Ȃ�
	if (mLerpFactor <= LERP_FACTOR_MIN)
	{
		// �ҋ@���[�V�����Z�b�g�AAngle��G�����ɖ߂�
		mMoveChara->SetMotion(Character::IDLE);
		mMoveChara->SetAngleY(mMoveChara->GetAngle().y + Define::PI); // �����𔽓]
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