#pragma once
#include <list>

#include "Enemy.h"

class EnemyManager
{
public:
	enum EnmList
	{
		DANBO = DataBase::ENM_ID_START,

	};

private:
	std::list<std::shared_ptr<Enemy>> mEnemies;
	Vector3 mPlayerPos;

public:
	EnemyManager() = default;
	~EnemyManager() = default;

	void Create(int charaID);
	void Destroy(int objID);

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir);

	int GetNum() const { return mEnemies.size(); }
	std::shared_ptr<Enemy> GetEnemy(int objID);

	void SetPlayerPos(const Vector3& pos) { mPlayerPos = pos; }
};