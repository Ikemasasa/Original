#pragma once
#include <memory>
#include <vector>

#include "lib/Vector.h"

class Terrain;

class CollisionTerrain
{
	static std::vector<Terrain*> mTerrains;

public:
	static int RayPickOrg(const Vector3& sp, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);
	static int RayPick(const Vector3& sp, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);
	
	static float GetHeight(const Vector3& pos, const float dist);
	static bool MoveCheck(const Vector3& pos, const Vector3& velocity, const float size, Vector3* outVelocity);
	static bool RegisterTerrain(Terrain* t);
	static bool UnRegisterTerrain(Terrain* t);
	static bool UnRegisterTerrainAll();
};