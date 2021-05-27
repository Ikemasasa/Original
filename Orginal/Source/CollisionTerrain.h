#pragma once
#include <memory>
#include <vector>

#include "lib/Vector.h"

class Terrain;

class CollisionTerrain
{
	static std::vector<std::shared_ptr<Terrain>> mTerrains;

public:
	static int RayPick(Vector3 sp, Vector3 ep, Vector3* outPos, Vector3* outNormal, float* outLen);
	static int MoveCheck(const Vector3& sp, const Vector3& ep, Vector3* outPos);
	static bool RegisterTerrain(const std::shared_ptr<Terrain>& t);
	static bool UnRegisterTerrain(const std::shared_ptr<Terrain>& t);
	static bool UnRegisterTerrainAll();
};