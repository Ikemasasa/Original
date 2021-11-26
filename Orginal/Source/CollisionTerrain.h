#pragma once
#include <memory>
#include <vector>

#include "lib/Vector.h"

class Terrain;

/*
	地形とのコリジョンをするクラス
	RegisterTerrainで登録したものとだけする
*/
class CollisionTerrain
{
	static std::vector<Terrain*> mTerrains;	// 登録した地形

public:
	// 
	static int RayPickOrg(const Vector3& sp, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);
	static int RayPick(const Vector3& sp, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);
	
	static float GetHeight(const Vector3& pos, const float dist);
	static bool MoveCheck(const Vector3& pos, const Vector3& velocity, const float size, Vector3* outVelocity);
	static bool RegisterTerrain(Terrain* t);
	static bool UnRegisterTerrain(Terrain* t);
	static bool UnRegisterTerrainAll();
};