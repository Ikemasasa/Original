#include "PlayerManager.h"

#include "lib/Input.h"
#include "lib/Matrix.h"
#include "lib/Vector.h"

#include "Fade.h"
#include "GameManager.h"
#include "Player.h"
#include "SceneManager.h"
#include "SceneMenu.h"
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
	mEquipmentInventory.Push(ItemData::BEGINNERS_SWORD);
	mEquipmentInventory.Push(ItemData::BEGINNERS_ARMOR);

	for (int i = DataBase::PL_ID_START; i < PL_ID_MAX; ++i)
	{
		Create(i);
		mPlayers.back()->Initialize();
	}

	mMovePlayer = mPlayers.back().get(); // 一番最初の要素
}

void PlayerManager::Update(const bool isTalking)
{
	// メニュー画面への遷移
	if (Input::GetButtonTrigger(0, Input::BUTTON::Y))
	{
		if (Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST))
		{
			Audio::SoundPlay((int)Sound::MENU_OPEN);
			SceneManager::GetInstance().SetStackScene(std::make_unique<SceneMenu>(this));
		}
	}

	// 会話中ならモーションのみ更新
	if (isTalking)
	{
		mMovePlayer->SetMotion(Character::IDLE);
		mMovePlayer->UpdateWorld();
	}
	else mMovePlayer->Update();

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

void PlayerManager::Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir)
{
	mMovePlayer->Render(shader, view, proj, lightDir);
}