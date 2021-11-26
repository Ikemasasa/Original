#include "ProductionGuard.h"

#include "lib/Matrix.h"

#include "BattleCharacter.h"
#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "DamageCalculator.h"
#include "Define.h"
#include "GameManager.h"
#include "Singleton.h"

void ProductionGuard::Initialize()
{
	mState = INIT;
	mFont.Initialize(64, 32);
}

void ProductionGuard::Update()
{
	BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);

	switch (mState)
	{
	case INIT:

		mMoveChara->SetMotion(Character::GUARD);
		mMoveChara->GetStatus()->SetGuardFlag(true);
		// break;

	case WAIT:
		// ガードの文字をセット
		Matrix view = Singleton<CameraManager>().GetInstance().GetView();
		Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();
		Vector2 scrPos = mMoveChara->GetTargetPos().WorldToScreen(view, proj);

		mFont.RenderSet(L"Guard", scrPos, Vector2(0.5f, 0.0f), Define::FONT_COLOR);

		// タイマーを進める
		StateWait();
	}

}

void ProductionGuard::Render()
{
	mFont.Render();
}
