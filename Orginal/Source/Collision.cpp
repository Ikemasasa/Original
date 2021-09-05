#include "Collision.h"
#include "Math.h"
#include "Terrain.h"
#include "lib/Math.h"

bool Collision::ColSpheres(const SPHERE& s1, const SPHERE& s2)
{
    Vector3 dist = s1.center - s2.center;
    float totalRadius = s1.radius + s2.radius;

    return (totalRadius * totalRadius) <= (dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
}

bool Collision::ColSphereAABB(const SPHERE& s, const AABB& aabb)
{
    float sphereCenter[3] = { s.center.x, s.center.y, s.center.z };
    float min[3] = { aabb.min.x, aabb.min.y, aabb.min.z };
    float max[3] = { aabb.max.x, aabb.max.y, aabb.max.z };

    float sqLength = 0.0f;
    for (int i = 0; i < 3; ++i)
    {
        // ���_��min, max�͈͓̔��ɂȂ�������
        // x, y, z ���ɒ������o���Ă�H
        if (sphereCenter[i] < min[i]) sqLength += (sphereCenter[i] - min[i]) * (sphereCenter[i] - min[i]);
        if (sphereCenter[i] > max[i]) sqLength += (sphereCenter[i] - max[i]) * (sphereCenter[i] - max[i]);
    }

    // �S��aabb�̒��ɓ����Ă邩��true
    if (sqLength == 0.0f) return true;

    return sqLength <= s.radius * s.radius;
}

bool Collision::ColSphereCapsule(const SPHERE& s, const CAPSULE c)
{
    Segment seg(c.centerTop, c.GetDistVector());
    float dist = Math::CalcPointSegmentDist(s.center, seg);
    return dist < s.radius + c.radius;
}

bool Collision::ColCapsules(const CAPSULE& c1, const CAPSULE& c2)
{
    Segment seg1(c1.centerTop, c1.GetDistVector());
    Segment seg2(c2.centerTop, c2.GetDistVector());
    float dist = Math::CalcSegmentSegmentDist(seg1, seg2);
    return dist < c1.radius + c2.radius;
}

bool Collision::ColAABBs(const AABB& aabb1, const AABB& aabb2)
{
    if (aabb1.min.x > aabb2.max.x) return false;
    if (aabb1.min.y > aabb2.max.y) return false;
    if (aabb1.min.z > aabb2.max.z) return false;
    if (aabb1.max.x < aabb2.min.x) return false;
    if (aabb1.max.y < aabb2.min.y) return false;
    if (aabb1.max.z < aabb2.min.z) return false;

    return true;
}