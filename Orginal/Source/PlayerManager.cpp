#include "PlayerManager.h"

#include "lib/Input.h"

#include "Fade.h"
#include "GameManager.h"
#include "Player.h"
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

	mMovePlayer = mPlayers.begin()->get(); // 一番最初の要素
	mMovePlayer->Initialize();
}

void PlayerManager::Update()
{
	// メニュー画面への遷移
	if (Input::GetButtonTrigger(0, Input::BUTTON::Y))
	{
		if (Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST))
		{
			SceneManager::GetInstance().SetStackScene(std::make_unique<SceneMenu>(this));
		}
	}

	mMovePlayer->Update();

	// 無敵時間の調整
	if (mIsInvincible)
	{
		mInvincibleTimer += GameManager::elapsedTime;

		// INVINCIBLE_SECOND秒経過後、無敵をoffにする
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