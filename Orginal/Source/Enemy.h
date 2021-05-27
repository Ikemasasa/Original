#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Actor.h"
#include "StatusData.h"

struct AABB;
class Player;

class Enemy : public Actor
{
	float mTimer = 0;
	Vector3 mVelocity;

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