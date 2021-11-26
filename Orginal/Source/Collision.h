#pragma once
#include <vector>
#include <memory>
#include "lib/Vector.h"
#include "lib/CollisionStructs.h"

class Terrain;

// �R���W�������������N���X
class Collision final
{
private:
	// ���ԍ쐬�A�J��
	Collision() = default;
	~Collision() = default;

public:
	static bool ColSpheres(const SPHERE& s1, const SPHERE& s2);
	static bool ColAABBs(const AABB& aabb1, const AABB& aabb2);
	static bool ColSphereAABB(const SPHERE& s, const AABB& aabb);
	static bool ColCapsules(const CAPSULE& c1, const CAPSULE& c2);
	static bool ColSphereCapsule(const SPHERE& s, const CAPSULE c);
	// static bool ColOBBs(const OBB& obb1, const OBB& obb2); �g��Ȃ����Ȃ������邩��Ƃ肠�����R�����g
};