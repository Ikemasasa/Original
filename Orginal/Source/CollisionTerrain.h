#pragma once
#include <memory>
#include <vector>

#include "lib/Vector.h"

class Terrain;

class CollisionTerrain
{
	static std::vector<Terrain*> mTerrains;

public:
	static int RayPick(const Vector3& sp, const Vector3& ep, Vector3* outPos, Vector3* outNormal, float* outLen);
	static int MoveCheck(const Vector3& sp, const Vector3& ep, Vector3* outPos);
	static bool RegisterTerrain(Terrain* t);
	static bool UnRegisterTerrain(Terrain* t);
	static bool UnRegisterTerrainAll();
};