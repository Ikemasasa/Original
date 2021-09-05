#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Character.h"
#include "StatusData.h"

class Enemy : public Character
{
	static constexpr float WAIT_TO_WALK_SEC = 7.0f;
	static constexpr float WALK_TO_WAIT_SEC = 3.0f;
	static constexpr float WALK_SPEED = 0.08f;
	static constexpr float RUN_SPEED = WALK_SPEED * 2.0f;
	static constexpr float MASS = 3.0f;

	enum MoveState
	{
		WAIT,
		WALK,
		CHASE,

		MAX
	};

	MoveState mState = WAIT;
	StatusData::EnemyType mEnmType;
	float mTimer = 0;
	float mMoveAngle; // 歩きの時の角度

	bool IsChase(const Vector3& playerPos);
	void Move(const Vector3& playerPos);
	void StateWait();
	void StateWalk();
	void StateChase(const Vector3& playerPos);
public:
	Enemy(int charaID);
	virtual ~Enemy();

	virtual void Initialize();
	virtual void Update(const Vector3& playerPos);
	
	StatusData::EnemyType GetEnmType() const { return mEnmType; }
	void SetEnmType(const StatusData::EnemyType& type) { mEnmType = type; }
};