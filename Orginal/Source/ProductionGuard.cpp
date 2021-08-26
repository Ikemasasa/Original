#include "ProductionGuard.h"

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
	mTimer = 0.0f;
	mFont.Initialize(64, 32);
}

void ProductionGuard::Update(const BattleCharacterManager* bcm)
{
	BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);

	switch (mState)
	{
	case INIT:
		mMoveChara = bcm->GetChara(mMoveCharaID);
		for (auto& targetID : mTargetCharaIDs)
		{
			mTargetCharas.push_back(bcm->GetChara(targetID));
		}

		mMoveChara->SetMotion(SkinnedMesh::GUARD);
		mMoveChara->GetStatus()->SetBuffDefRate(DamageCalculator::DEFENCE_VIT_RATE, 1);
		// break;

	case WAIT:
		// ガードの文字をセット
		DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
		DirectX::XMFLOAT4X4 proj = Singleton<CameraManager>().GetInstance().GetProj();
		Vector3 pos = mMoveChara->GetPos();
		pos.y += (mMoveChara->GetLocalAABB().max.y - mMoveChara->GetLocalAABB().min.y) / 2.0f;
		Vector2 scrPos = pos.WorldToScreen(view, proj);
		const wchar_t* str = L"Guard";
		mFont.RenderSet(str, scrPos, Vector2(mFont.GetWidth(str) / 2.0f, 0.0f), Define::FONT_COLOR);

		// タイマーを進める
		mTimer += GameManager::elapsedTime;
		if (mTimer >= WAIT_SEC)
		{
			mIsFinished = true;
		}
	}

}

void ProductionGuard::Render()
{
	mFont.Render();
}
