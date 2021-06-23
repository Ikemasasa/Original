#include "PlayerManager.h"

#include "lib/Input.h"

#include "Fade.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "ItemData.h"

void PlayerManager::Create(int charaID)
{
	mPlayers.emplace_back(std::make_unique<Player>(charaID));
	mPlayers.back()->SetObjID(mPlayers.size() - 1);
}

void PlayerManager::Destroy(int objID)
{
	// わかりやすくするために2つに分割
	auto it = mPlayers.begin();
	while (it != mPlayers.end())
	{
		if (it->get()->GetObjID() == objID)
		{
			it = mPlayers.erase(it);
			break;
		}

		++it;
	}
	while (it != mPlayers.end())
	{
		// 消したエネミー以降のエネミーのobjidを-1 
		// objID = mPlayers.size() を成り立たせるため
		auto p = it->get();
		p->SetObjID(p->GetObjID() - 1);
	}
}

void PlayerManager::Initialize()
{
	mMovePlayer = mPlayers.begin()->get(); // 一番最初の要素
	mMovePlayer->Initialize();
}

void PlayerManager::Update()
{
	if (Input::GetButtonTrigger(0, Input::BUTTON::Y))
	{
		if (Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST))
		{
			SceneManager::GetInstance().SetStackScene(std::make_unique<SceneMenu>(this));
		}
	}

	mMovePlayer->Update();
}

void PlayerManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir)
{
	mMovePlayer->Render(view, proj, lightDir);
}

void PlayerManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir)
{
	mMovePlayer->Render(shader, view, proj, lightDir);
}