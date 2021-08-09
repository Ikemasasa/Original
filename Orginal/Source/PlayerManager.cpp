#include "PlayerManager.h"

#include "lib/Input.h"

#include "DataBase.h"
#include "Fade.h"
#include "GameManager.h"
#include "Player.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "Singleton.h"
#include "ItemData.h"

PlayerManager::PlayerManager()
{
	mPlayers.clear();
}

PlayerManager::~PlayerManager()
{
	mPlayers.clear();
}

void PlayerManager::Create(int charaID)
{
	mPlayers.emplace_back(std::make_unique<Player>(charaID));
	mPlayers.back()->SetObjID(mPlayers.size() - 1);
}

void PlayerManager::Initialize()
{
	mEquipmentInventory.Push(EquipmentData::BEGINNERS_SWORD);
	mEquipmentInventory.Push(EquipmentData::BEGINNERS_ARMOR);

	for (int i = DataBase::PL_ID_START; i < PL_ID_MAX; ++i)
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
		mInvincibleTimer += GameManager::elapsedTime;

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