#pragma once
#include <vector>
#include <memory>
#include "lib/Vector.h"
#include "lib/CollisionStructs.h"

class Terrain;

// コリジョン判定をするクラス
class Collision final
{
private:
	// 実態作成アカン
	Collision() = default;
	~Collision() = default;

public:
	static bool ColSpheres(const SPHERE& s1, const SPHERE& s2);
	static bool ColAABBs(const AABB& aabb1, const AABB& aabb2);
	static bool ColSphereAABB(const SPHERE& s, const AABB& aabb);
	static bool ColCapsules(const CAPSULE& c1, const CAPSULE& c2);
	static bool ColSphereCapsule(const SPHERE& s, const CAPSULE c);
	// static bool ColOBBs(const OBB& obb1, const OBB& obb2); 使わなそうなきがするからとりあえずコメント
};