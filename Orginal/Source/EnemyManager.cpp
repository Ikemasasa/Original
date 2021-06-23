#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	mEnemies.clear();
}

EnemyManager::~EnemyManager()
{
	mEnemies.clear();
}

void EnemyManager::Create(int charaID)
{
	mEnemies.push_back(std::make_unique<Enemy>(charaID));
	mEnemies.back()->SetObjID(mEnemies.size() - 1);
}

void EnemyManager::Destroy(int objID)
{
	// わかりやすくするために2つに分割

	auto it = mEnemies.begin();
	while (it != mEnemies.end())
	{
		if (it->get()->GetObjID() == objID)
		{
			it = mEnemies.erase(it);
			break;
		}

		++it;
	}
	while (it != mEnemies.end())
	{
		// 消したエネミー以降のエネミーのobjidを-1 
		// objID = mEnemies.size() を成り立たせるため
		auto e = it->get();
		e->SetObjID(e->GetObjID() - 1);
	}
}

void EnemyManager::Initialize()
{
	for (auto& enm : mEnemies) enm->Initialize();
}

void EnemyManager::Update()
{
	for (auto& enm : mEnemies) enm->Update(mPlayerPos); 
}

void EnemyManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& enm : mEnemies) enm->Render(view, proj, lightDir);
}

void EnemyManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& enm : mEnemies) enm->Render(shader, view, proj, lightDir);
}

Enemy* EnemyManager::GetEnemy(int objID)
{
	for (auto& enm : mEnemies)
	{
		if (enm->GetObjID() != objID) continue;

		return enm.get();
	}

	return nullptr; // 見つからなかった
}