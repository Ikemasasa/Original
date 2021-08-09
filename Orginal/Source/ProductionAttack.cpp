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
	case State::INIT: // ������ �_���[�W�v�Z
	{
		// moveChara, targetChara���
		mMoveChara = bcm->GetChara(mMoveCharaID);
		mTargetChara = bcm->GetChara(mTargetCharaID);

		// �_���[�W�v�Z
		int damage = DamageCalculator::CalcAttackDamage(mMoveChara->GetStatus(), mTargetChara->GetStatus());
		mTargetChara->GetStatus()->HurtHP(damage);
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
	// MoveChara�̍��W�����_�ɂ���
	mOrgPos = mMoveChara->GetPos();

	// TargetChara ���� MoveChara �̃x�N�g��������āA�s��������߂�(mDestinationPos)
	Vector3 targetToOrg = mOrgPos - mTargetChara->GetPos();
	targetToOrg.Normalize();

	const float ADJUST_DIST_MUL = 4;
	mDestinationPos = mTargetChara->GetPos() + targetToOrg * mTargetChara->GetCapsule().radius * ADJUST_DIST_MUL;

	// �s�������Ɍ���
	mMoveChara->CorrectionAngle(-targetToOrg);

	++mState; // �X�e�[�g�����ɐi�߂�
}

void ProductionAttack::StateMoveToTarget()
{
	// ���胂�[�V�����Z�b�g
	mMoveChara->SetMotion(SkinnedMesh::RUN);

	// mOrgPos ���� mDestinationPos �܂ł���`�⊮����
	mLerpFactor = Math::Min(mLerpFactor + LERP_FACTOR_ADD, LERP_FACTOR_MAX);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveChara->SetPos(lerp);

	// mLerpFactor��1�ȏ�Ȃ�
	if (mLerpFactor >= LERP_FACTOR_MAX)
	{
		// �U�����[�V�����Z�b�g
		mMoveChara->SetMotion(SkinnedMesh::ATTACK, false);
		++mState;
	}
}

void ProductionAttack::StateWaitAttack()
{
	// ���[�V�������I�������
	if (mMoveChara->IsMotionFinished())
	{
		if (mTargetChara->GetStatus()->IsDead())
		{
			// Exist��false�ɂ��āA�G�t�F�N�g���Đ�����
			mTargetChara->SetExist(false); // �����Ȃ�����
			Vector3 effectPos(mTargetChara->GetPos().x, mTargetChara->GetPos().y + mTargetChara->GetLocalAABB().max.y * 0.5f, mTargetChara->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// �����ӂ�����
		}

		Vector3 damagePos(mTargetChara->GetPos().x, mTargetChara->GetAABB().max.y, mTargetChara->GetPos().z);
		mProductionValue.Add(mAmount, damagePos, DAMAGE_RGB);

		// �����������Ɍ���
		mMoveChara->SetAngleY(mMoveChara->GetAngle().y + Define::PI); // �����𔽓]
		++mState;
	}
}

void ProductionAttack::StateMoveToOrigin()
{
	// ���胂�[�V�����Z�b�g
	mMoveChara->SetMotion(SkinnedMesh::RUN);

	// mDestinationPos ���� mOrgPos �܂ł���`�⊮����
	mLerpFactor = Math::Max(mLerpFactor - LERP_FACTOR_ADD, LERP_FACTOR_MIN);
	Vector3 lerp = Vector3::Lerp(mOrgPos, mDestinationPos, mLerpFactor);
	mMoveChara->SetPos(lerp);

	// mLerpFactor��0�ȉ��Ȃ�
	if (mLerpFactor <= LERP_FACTOR_MIN)
	{
		// �ҋ@���[�V�����Z�b�g�AAngle��G�����ɖ߂�
		mMoveChara->SetMotion(SkinnedMesh::IDLE);
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