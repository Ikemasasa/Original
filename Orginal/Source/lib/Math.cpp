#include "Math.h"

bool Math::IsSharpAngle(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	const Vector3 vec1 = p1 - p2;
	const Vector3 vec2 = p3 - p2;
	return Vector3::Dot(vec1, vec2) >= 0.0f;
}

bool Math::IsParallel(const Vector3& v1, const Vector3& v2)
{
	Vector3 cross = Vector3::Cross(v1, v2);
	float d = cross.LengthSq();

	return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// �덷�͈͓��Ȃ畽�s�Ɣ���
}

float Math::CalcPointLineDist(const Vector3& p, const Line& line, Vector3& h, float& t)
{
	float lengthSqV = line.v.LengthSq();
	t = 0.0f;
	if (lengthSqV > 0.0f)
	{
		t = Vector3::Dot(line.v, Vector3(p- line.p)) / lengthSqV;
	}

	h = line.GetPoint(t);
	return Vector3(h - p).Length();
}

inline float Math::CalcPointLineDist(const Vector3& p, const Line& line)
{
	Vector3 h = Vector3::ZERO;
	float t = 0.0f;
	return CalcPointLineDist(p, line, h, t);
}

float Math::CalcPointSegmentDist(const Vector3& p, const Segment& seg, Vector3& h, float& t)
{
	const Vector3 e = seg.GetEndPoint();

	// �����̒����A�����̑��̍��W�y�� t ���Z�o
	float len = CalcPointLineDist(p, Line(seg.p, e - seg.p), h, t);

	if (IsSharpAngle(p, seg.p, e) == false)
	{
		// �n�_���̊O��
		h = seg.p;
		return Vector3(seg.p - p).Length();
	}
	else if (IsSharpAngle(p, e, seg.p) == false)
	{
		// �I�_���̊O��
		h = e;
		return Vector3(e - p).Length();
	}

	return len;
}

float Math::CalcPointSegmentDist(const Vector3& p, const Segment& seg)
{
	Vector3 h = Vector3::ZERO;
	float t = 0.0f;
	return CalcPointLineDist(p, seg, h, t);
}

float Math::CalcLineLineDist(const Line& line1, const Line& line2, Vector3& p1, Vector3& p2, float& t1, float& t2)
{
	// 2���������s�H
	if (IsParallel(line1.v, line2.v) == true)
	{
		// �_P11�ƒ���L2�̍ŒZ�����̖��ɋA��
		float len = CalcPointLineDist(line1.p, line2, p2, t2);
		p1 = line1.p;
		t1 = 0.0f;

		return len;
	}

	// 2�����͂˂���֌W
	float DV1V2 = Vector3::Dot(line1.v, line2.v);
	float DV1V1 = line1.v.LengthSq();
	float DV2V2 = line2.v.LengthSq();
	Vector3 P21P11 = line1.p - line2.p;
	t1 = (DV1V2 * Vector3::Dot(line2.v, P21P11) - DV2V2 * Vector3::Dot(line1.v, P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
	p1 = line1.GetPoint(t1);
	t2 = Vector3::Dot(line2.v, (p1 - line2.p)) / DV2V2;
	p2 = line2.GetPoint(t2);

	return Vector3(p2 - p1).Length();
}

float Math::CalcLineLineDist(const Line& line1, const Line& line2)
{
	Vector3 p1, p2;
	float t1, t2;
	return CalcLineLineDist(line1, line2, p1, p2, t1, t2);
}

float Math::CalcSegmentSegmentDist(const Segment& seg1, const Segment& seg2, Vector3& p1, Vector3& p2, float& t1, float& t2)
{
	// S1 ���k�ނ��Ă���H
	if (seg1.v.LengthSq() < _OX_EPSILON_)
	{
		// S2���k��
		if (seg2.v.LengthSq() < _OX_EPSILON_)
		{
			// �_�Ɠ_�̋����̖��ɋA��
			float len = Vector3(seg2.p - seg1.p).Length();
			p1 = seg1.p;
			p2 = seg2.p;
			t1 = t2 = 0.0f;
			return len;
		}
		else
		{
			// S1 �̎n�_�� S2 �̍ŒZ���ɋA��
			float len = CalcPointSegmentDist(seg1.p, seg2, p2, t2);
			p1 = seg1.p;
			t1 = 0.0f;
			Clamp01(t2);
			return len;
		}
	}
	// S2 ���k�ނ��Ă���H
	else if (seg2.v.LengthSq() < _OX_EPSILON_)
	{
		// S2 �̎n�_�� S1 �̍ŒZ���ɋA��
		float len = CalcPointSegmentDist(seg2.p, seg1, p1, t1);
		p2 = seg2.p;
		Clamp01(t1);
		t2 = 0.0f;
		return len;
	}


	//////////	 �������m   ///////////

	// 2���������s�������琂���̒[�_�̈��P1�ɉ�����
	if (IsParallel(seg1.v, seg2.v) == true)
	{
		t1 = 0.0f;
		p1 = seg1.p;
		float len = CalcPointSegmentDist(seg1.p, seg2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
		{
			return len;
		}
	}
	else
	{
		// �����͂˂���̊֌W
		// 2�����Ԃ̍ŒZ���������߂ĉ��� t1, t2 �����߂�
		float len = CalcLineLineDist(Line(seg1.p, seg1.GetEndPoint() - seg1.p), Line(seg2.p, seg2.GetEndPoint() - seg2.p), p1, p2, t1, t2);
		if (0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f)
		{
			return len;
		}
	}

	// �����̑����O�ɂ��邱�Ƃ�����
	// S1����t1��0~1�̊ԂɃN�����v���Đ��������낷
	Clamp01(t1);
	p1 = seg1.GetPoint(t1);
	float len = CalcPointSegmentDist(p1, seg2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
	{
		return len;
	}

	// S2���킪���Ƃ������̂�S2�����N�����v�AS1�ɐ������~�낷
	Clamp01(t2);
	p2 = seg2.GetPoint(t2);
	len = CalcPointSegmentDist(p2, seg1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
	{
		return len;
	}

	// �o���̒[�_���ŒZ�Ɣ���
	Clamp01(t1);
	p1 = seg1.GetPoint(t1);
	return Vector3(p2 - p1).Length();
}

float Math::CalcSegmentSegmentDist(const Segment& seg1, const Segment& seg2)
{
	Vector3 p1, p2;
	float t1, t2;
	return CalcSegmentSegmentDist(seg1, seg2, p1, p2, t1, t2);
}

