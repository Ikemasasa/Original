#include "EnemyManager.h"

#include "lib/Shader.h"

#include "BossEnemy.h"
#include "Enemy.h"
#include "GameManager.h"
#include "StatusData.h"

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
	StatusData::EnemyType type = StatusData::GetEnmType(charaID);
	switch (type)
	{
	case StatusData::MOB:  mEnemies.push_back(std::make_unique<Enemy>(charaID)); break;
	case StatusData::BOSS: mEnemies.push_back(std::make_unique<BossEnemy>(charaID)); break;
	}

	mEnemies.back()->SetEnmType(type);
	mEnemies.back()->SetObjID(mEnemies.size() - 1);
}

std::list<std::unique_ptr<Enemy>>::iterator EnemyManager::Destroy(int objID)
{
	// わかりやすくするために2つに分割

	auto ret = mEnemies.begin();
	while (ret != mEnemies.end())
	{
		if (ret->get()->GetObjID() == objID)
		{
			ret = mEnemies.erase(ret);
			break;
		}

		++ret;
	}

	auto it = ret;
	while (it != mEnemies.end())
	{
		// 消したエネミー以降のエネミーのobjidを-1 
		// objID = mEnemies.size() を成り立たせるため
		auto e = it->get();
		e->SetObjID(e->GetObjID() - 1);

		++it;
	}

	return ret;
}

void EnemyManager::Initialize()
{
	for (int i = DataBase::ENM_ID_START; i < ENM_NUM; ++i)
	{
		Create(i);
	}

	for (auto& enm : mEnemies) enm->Initialize();
}

void EnemyManager::Update(const bool isTalking)
{
	for (auto it = mEnemies.begin(); it != mEnemies.end();)
	{
		auto& enm = *it;

		// 会話中ならモーションの更新だけする
		if (isTalking) enm->UpdateWorld();
		else		   enm->Update(mPlayerPos);

		if (enm->GetExist() == false)
		{
			if (enm->GetEnmType() == StatusData::EnemyType::BOSS) GameManager::bossSubdueFlag = true;
			it = Destroy(enm->GetObjID());
		}
		else
		{
			++it;
		}
	}
}

void EnemyManager::Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir)
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