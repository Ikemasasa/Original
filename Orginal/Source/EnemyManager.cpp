#include "EnemyManager.h"

void EnemyManager::Create(int charaID)
{
	std::shared_ptr<Enemy> e = std::make_shared<Enemy>(charaID);

	e->SetObjID(mEnemies.size());
	mEnemies.emplace_back(e);
}

void EnemyManager::Destroy(int objID)
{
	// �킩��₷�����邽�߂�2�ɕ���

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
		// �������G�l�~�[�ȍ~�̃G�l�~�[��objid��-1 
		// objID = mEnemies.size() �𐬂藧�����邽��
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

std::shared_ptr<Enemy> EnemyManager::GetEnemy(int objID)
{
	std::shared_ptr<Enemy> ret = nullptr;
	for (auto& enm : mEnemies)
	{
		if (enm->GetObjID() != objID) continue;

		ret = enm;
		break;
	}

	return ret;
}