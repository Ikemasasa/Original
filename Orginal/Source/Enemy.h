#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Character.h"


class Enemy : public Character
{
	static constexpr float WAIT_TO_WALK_SEC = 5;
	static constexpr float WALK_TO_WAIT_SEC = 4;
	static constexpr float WALK_SPEED = 0.08f;
	static constexpr float RUN_SPEED = WALK_SPEED * 2.0f;
	static constexpr float MASS = 10;

	enum MoveState
	{
		WAIT,
		WALK,
		CHASE,

		MAX
	};

	MoveState mState = WAIT;
	float mTimer = 0;
	float mMoveAngle; // •à‚«‚ÌŽž‚ÌŠp“x

	bool IsChase(const Vector3& playerPos);
	void Move(const Vector3& playerPos);
	void StateWait();
	void StateWalk();
	void StateChase(const Vector3& playerPos);
public:
	Enemy(int charaID);
	~Enemy();

	void Initialize();
	void Update(const Vector3& playerPos);

};