#include "Collision.h"
#include "Math.h"
#include "Terrain.h"
#include "lib/Math.h"

FLOAT LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);

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

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
FLOAT LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3)
{
    // 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
    // ������Sep�͕W��������Ă��邱��
    FLOAT r1 = static_cast<float>(fabs(Sep->Dot(*e1)));
    FLOAT r2 = static_cast<float>(fabs(Sep->Dot(*e2)));
    FLOAT r3 = e3 ? static_cast<float>((fabs(Sep->Dot(*e3)))) : 0;
    return r1 + r2 + r3;
}

//bool Collision::ColOBBs(const OBB& obb1, const OBB& obb2)
//{
//    // �e�����x�N�g���̊m��
//    // �iN***:�W���������x�N�g���j
//    Vector3 NAe1 = obb1.norDirection[0], Ae1 = NAe1 * obb1.length.x;
//    Vector3 NAe2 = obb1.norDirection[1], Ae2 = NAe2 * obb1.length.y;
//    Vector3 NAe3 = obb1.norDirection[2], Ae3 = NAe3 * obb1.length.z;
//    Vector3 NBe1 = obb2.norDirection[0], Be1 = NBe1 * obb2.length.x;
//    Vector3 NBe2 = obb2.norDirection[1], Be2 = NBe2 * obb2.length.y;
//    Vector3 NBe3 = obb2.norDirection[2], Be3 = NBe3 * obb2.length.z;
//    Vector3 Interval = obb1.center - obb2.center;
//
//    // ������ : Ae1
//    FLOAT rA = Ae1.Length();
//    FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
//    FLOAT L = static_cast<float>(fabs(Interval.Dot(NAe1)));
//    if (L > rA + rB) return false; // �Փ˂��Ă��Ȃ�
//
//     // ������ : Ae2
//    rA = Ae2.Length();
//    rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
//    L = static_cast<float>(fabs(Interval.Dot(NAe2)));
//    if (L > rA + rB) return false;
//
//    // ������ : Ae3
//    rA = Ae3.Length();
//    rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
//    L = static_cast<float>(fabs(Interval.Dot(NAe3)));
//    if (L > rA + rB) return false;
//
//    // ������ : Be1
//    rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
//    rB = Be1.Length();
//    L = static_cast<float>(fabs(Interval.Dot(NBe1)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : Be2
//    rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
//    rB = Be2.Length();
//    L = static_cast<float>(fabs(Interval.Dot(NBe2)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : Be3
//    rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
//    rB = Be3.Length();
//    L = static_cast<float>(fabs(Interval.Dot(NBe3)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : C11
//    Vector3 Cross;
//    Cross = NAe1.Cross(NBe1);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//    rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB) return false;
//
//    // ������ : C12
//    Cross = NAe1.Cross(NBe2);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//    rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : C13
//    Cross = NAe1.Cross(NBe3);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//    rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : C21
//    Cross = NAe2.Cross(NBe1);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//    rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : C22
//    Cross = NAe2.Cross(NBe2);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//    rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : C23
//    Cross = NAe2.Cross(NBe3);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//    rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : C31
//    Cross = NAe3.Cross(NBe1);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//    rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : C32
//    Cross = NAe3.Cross(NBe2);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//    rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB)
//        return false;
//
//    // ������ : C33
//    Cross = NAe3.Cross(NBe3);
//    rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//    rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//    L = static_cast<float>(fabs(Interval.Dot(Cross)));
//    if (L > rA + rB) return false;
//
//    // �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
//    return true;
//}