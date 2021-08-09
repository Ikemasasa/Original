#include "ProductionEscape.h"

#include "BattleCharacterManager.h"
#include "Fade.h"
#include "SceneManager.h"

void ProductionEscape::Initialize()
{
	mState = 0;
}

void ProductionEscape::Update(const BattleCharacterManager* bcm)
{
	switch (mState)
	{
	case INIT:
		if (Fade::GetInstance().Set(Fade::SPEED_SLOW))
		{
			mMoveChara = bcm->GetMoveChara();
			float vx = sinf(mMoveChara->GetAngle().y) * MOVE_SPEED;
			float vz = cosf(mMoveChara->GetAngle().y) * MOVE_SPEED;
			mMoveChara->SetVelocity(Vector3(-vx, 0.0f, -vz));
			mMoveChara->SetMotion(SkinnedMesh::RUN);
			++mState;
		}
		// break;

	case FADE_OUT:
		if (Fade::GetInstance().IsFadeOutEnd())
		{
			// ˆÓ–¡‚Í‚È‚¢‚¯‚ÇAˆê‰žƒ[ƒ‚É‚µ‚Æ‚­
			mMoveChara->SetVelocity(Vector3::ZERO);
			mIsFinished = true;

			SceneManager::GetInstance().PopCurrentScene();
		}
		

		mMoveChara->SetPos(mMoveChara->GetPos() + mMoveChara->GetVelocity());
		mMoveChara->CorrectionAngle();

		break;
	}
}