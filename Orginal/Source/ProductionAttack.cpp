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
	case State::INIT: // ������ �_���[�W�v�Z
	{
		// moveactor, targetactor���
		mMoveActor = bam->GetActor(mMoveActorID);
		mTargetActor = bam->GetActor(mTargetActorID);

		// �_���[�W�v�Z
		int damage = CalcDamage(mMoveActor->GetStatus(), mTargetActor->GetStatus());
		mTargetActor->GetStatus()->HurtHP(damage);
		mAmount = damage;
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
	// MoveActor�̍��W�����_�ɂ���
	mOrgPos = mMoveActor->GetPos();

	// TargetActor ���� MoveActor �̃x�N�g��������āA�s��������߂�(mDestinationPos)
	const float ADJUST_DIST_MUL = 2;
	Vector3 targetToOrg = mOrgPos - mTargetActor->GetPos();
	targetToOrg.Normalize();
	//if (targetToOrg == Vector3::Zero()) targetToOrg = Vector3(sinf(targetActor->GetAngle().y), targetActor->GetPos().y, cosf(targetActor->GetAngle().y));
	//else							    

	mDestinationPos = mTargetActor->GetPos() + targetToOrg * mTargetActor->GetCapsule().radius * ADJUST_DIST_MUL;

	++mState; // �X�e�[�g�����ɐi�߂�
}

void ProductionAttack::StateMoveToTarget()
{
	// ���胂�[�V�����Z�b�g
	mMoveActor->SetMotion(SkinnedMesh::RUN);

	// mOrgPos ���� mDestinationPos �܂ł���`�⊮����
	mLerpFactor = Math::Min(mLerpFactor + LERP_FACTOR_ADD, LERP_FACTOR_MAX);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveActor->SetPos(lerp);

	// mLerpFactor��1�ȏ�Ȃ�
	if (mLerpFactor >= LERP_FACTOR_MAX)
	{
		// �U�����[�V�����Z�b�g
		mMoveActor->SetMotion(SkinnedMesh::ATTACK, false);
		++mState;
	}
}

void ProductionAttack::StateWaitAttack()
{
	// ���[�V�������I�������
	if (mMoveActor->IsMotionFinished())
	{
		if (mTargetActor->GetStatus()->IsDead()) // �U���Ώۂ�����ł���
		{
			// �����Ȃ����āA�G�t�F�N�g�Đ�
			mTargetActor->SetExist(false); // �����Ȃ�����
			Vector3 effectPos(mTargetActor->GetPos().x, mTargetActor->GetPos().y + mTargetActor->GetLocalAABB().max.y * 0.5f, mTargetActor->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// �����ӂ�����
		}
		Vector3 damagePos(mTargetActor->GetPos().x, mTargetActor->GetAABB().max.y, mTargetActor->GetPos().z);
		Vector3 damageRGB(1.0f, 0.4f, 0.4f);
		Vector2 center(0.0f, 0.0f);
		Vector2 scale(1.0f, 1.0f);
		mProductionValue.Add(mAmount, damagePos, damageRGB);

		// �����������Ɍ���
		Vector3 dist = mOrgPos - mDestinationPos; // �������������x�N�g��
		dist.Normalize();
		mMoveActor->CorrectionAngle(dist);
		++mState;
	}
}

void ProductionAttack::StateMoveToOrigin()
{
	// ���胂�[�V�����Z�b�g
	mMoveActor->SetMotion(SkinnedMesh::RUN);

	// mDestinationPos ���� mOrgPos �܂ł���`�⊮����
	mLerpFactor = Math::Max(mLerpFactor - LERP_FACTOR_ADD, LERP_FACTOR_MIN);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveActor->SetPos(lerp);

	// mLerpFactor��0�ȉ��Ȃ�
	if (mLerpFactor <= LERP_FACTOR_MIN)
	{
		// �ҋ@���[�V�����Z�b�g�AAngle��G�����ɖ߂�
		mMoveActor->SetMotion(SkinnedMesh::IDLE);
		mMoveActor->SetAngleY(mMoveActor->GetAngle().y + Define::PI); // �����𔽓]
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
	int damage = deal->str / 2 - take->vit / 4; // ��b�_���[�W
	int sign = (rand() % 2 == 0) ? -1 : 1; // �U�ꕝ�̕���
	int width = damage / 16 + 1; // �_���[�W�̐U�ꕝ�̍ő�l
	damage = damage + (rand() % width * sign);

	if (damage < 0) damage = 0;

	return damage;
}