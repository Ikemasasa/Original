#pragma once
#include <list>

#include "lib/Vector.h"

#include "DataBase.h"

class Enemy;
class Shader;

class EnemyManager
{
public:
	enum EnmList
	{
		MONSTER_A = DataBase::ENM_ID_START,
		MONSTER_B,
		BOSS_MONSTER,

		ENM_NUM
	};

private:
	std::list<std::unique_ptr<Enemy>> mEnemies;
	Vector3 mPlayerPos = {};

public:
	EnemyManager();
	~EnemyManager();

	void Create(int charaID);
	std::list<std::unique_ptr<Enemy>>::iterator Destroy(int objID);

	void Initialize();
	void Update(const bool isTalking);
	void Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir);

	int GetNum() const { return mEnemies.size(); }
	Enemy* GetEnemy(int objID);

	void SetPlayerPos(const Vector3& pos) { mPlayerPos = pos; }
};