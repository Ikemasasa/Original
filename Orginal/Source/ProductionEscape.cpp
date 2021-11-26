#include "ProductionEscape.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "Fade.h"
#include "SceneManager.h"

void ProductionEscape::Initialize()
{
	mState = 0;
}

void ProductionEscape::Update()
{
	switch (mState)
	{
	case INIT:

		if (Fade::GetInstance().Set(Fade::SPEED_SLOW))
		{
			BattleState::GetInstance().SetState(BattleState::State::ESCAPE);

			float vx = sinf(mMoveChara->GetAngle().y) * MOVE_SPEED;
			float vz = cosf(mMoveChara->GetAngle().y) * MOVE_SPEED;
			mMoveChara->SetVelocity(Vector3(-vx, 0.0f, -vz));
			mMoveChara->SetMotion(Character::RUN);
			++mState;
		}
		// break;

	case FADE_OUT:

		mMoveChara->SetPos(mMoveChara->GetPos() + mMoveChara->GetVelocity());
		mMoveChara->CorrectionAngle();

		if (Fade::GetInstance().IsFadeOutEnd())
		{
			// ˆÓ–¡‚Í‚È‚¢‚¯‚ÇAˆê‰žƒ[ƒ‚É‚µ‚Æ‚­
			mMoveChara->SetVelocity(Vector3::ZERO);
			mIsFinished = true;

			SceneManager::GetInstance().PopCurrentScene();
		}

		break;
	}
}