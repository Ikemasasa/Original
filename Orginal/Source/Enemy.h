#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Character.h"


class Enemy : public Character
{
	float mTimer = 0;
	enum MoveState
	{
		WAIT,
		WALK,
		CHASE,

		MAX
	};

	MoveState mState = WAIT;

	void DecideMoveState(const Vector3& playerPos);
public:
	Enemy(int charaID);
	~Enemy();

	void Initialize();
	void Update(const Vector3& playerPos);

};