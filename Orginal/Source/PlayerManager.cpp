#include "PlayerManager.h"

#include "lib/Input.h"

#include "Fade.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "ItemData.h"

void PlayerManager::Create(int charaID)
{
	mPlayers.emplace_back(std::make_unique<Player>(charaID));
	mPlayers.back()->SetObjID(mPlayers.size() - 1);
}

void PlayerManager::Initialize()
{
	for (int i = DataBase::PL_ID_START; i < PL_NUM; ++i)
	{
		Create(i);
		mPlayers.back()->Initialize();
	}

	mMovePlayer = mPlayers.begin()->get(); // ˆê”ÔÅ‰‚Ì—v‘f
	mMovePlayer->Initialize();
}

void PlayerManager::Update()
{
	// ƒƒjƒ…[‰æ–Ê‚Ö‚Ì‘JˆÚ
	if (Input::GetButtonTrigger(0, Input::BUTTON::Y))
	{
		if (Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST))
		{
			SceneManager::GetInstance().SetStackScene(std::make_unique<SceneMenu>(this));
		}
	}

	mMovePlayer->Update();

	// –³“GŠÔ‚Ì’²®
	if (mIsInvincible)
	{
		mInvincibleTimer += GameManager::elpsedTime;

		// INVINCIBLE_SECOND•bŒo‰ßŒãA–³“G‚ğoff‚É‚·‚é
		if (mInvincibleTimer >= INVINCIBLE_SECOND)
		{
			mIsInvincible = false;
			mInvincibleTimer = 0.0f;
		}
	}
}

void PlayerManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir)
{
	mMovePlayer->Render(view, proj, lightDir);
}

void PlayerManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir)
{
	mMovePlayer->Render(shader, view, proj, lightDir);
}