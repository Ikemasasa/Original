#pragma once
#include "Vector.h"

#define _OX_EPSILON_	0.000001f	// �덷

struct Line
{
	Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
	Line(const Vector3& p, const Vector3& v) : p(p), v(v) {}

	Vector3 p; // �n�_
	Vector3 v; // �����x�N�g��

	// �_��̍��W���擾
	//  �x�N�g���Ɋ|���Z����W��
	Vector3 GetPoint(float t) const { return p + (v * t); }
};

struct Segment : public Line
{
	Segment() : Line() {}
	Segment(const Vector3& p, const Vector3& v) : Line(p, v) {}

	// ���������Ă��悫
	// Vector3 len; // �����x�N�g���̒���

	// �I�_���l��
	Vector3 GetEndPoint() const { return p + v; }
};

namespace Math
{
	template <typename T>
	constexpr T Max(const T& value1, const T& value2)
	{
		return (value1 > value2) ? value1 : value2;
	}

	template <typename T>
	constexpr T Min(const T& value1, const T& value2)
	{
		return (value1 > value2) ? value2 : value1;
	}

	template <typename T>
	constexpr T Clamp(const T& value, const T& min, const T& max)
	{
		return Min(Max(value, min), max);
	}

	constexpr float Clamp01(const float& value)
	{
		return Min(Max(value, 0.0f), 1.0f);
	}

	template <typename T>
	constexpr T Lerp(const T& value1, const T& value2, const float& t)
	{
		return (1.0f - t) * value1 + t * value2;
	}

	constexpr float ToRadian(const float& degree)
	{
		return degree * 0.01745f;
	}

	bool IsSharpAngle(const Vector3& p1, const Vector3& p2, const Vector3& p3);	// ��p1p2p3 ���s�p(0�`90�x)��
	bool IsParallel(const Vector3& v1, const Vector3& v2); // ��{�̃x�N�g�������s��

	// �_�ƒ����̍ŒZ����
	// p	: �_
	// line : ����
	// h	: �_����~�낵�������̑�
	// t	: �x�N�g���W��
	// �߂�l : �ŒZ����
	float CalcPointLineDist(const Vector3& p, const Line& line, Vector3& h, float& t);
	float CalcPointLineDist(const Vector3& p, const Line& line);

	// �_�Ɛ����̍ŒZ����
	// p   : �_
	// seg : ����
	// h   : �ŒZ�����ƂȂ�[�_
	// t   : �[�_�ʒu(�@t < 0: �n�_�̊O, 0 <= t <= 1: ������, t > 1: �I�_�̊O�@)
	// �߂�l : �ŒZ����
	float CalcPointSegmentDist(const Vector3& p, const Segment& seg, Vector3& h, float& t);
	float CalcPointSegmentDist(const Vector3& p, const Segment& seg);

	// 2�����̍ŒZ����
	// line1 : �����P
	// line2 : �����Q
	// p1 : L1���̐����̑��i�߂�l�j
	// p2 : L2���̐����̑��i�߂�l�j
	// t1 : L1���̃x�N�g���W���i�߂�l�j
	// t2 : L2���̃x�N�g���W���i�߂�l�j
	// �߂�l : �ŒZ����
	float CalcLineLineDist(const Line& line1, const Line& line2, Vector3& p1, Vector3& p2, float& t1, float& t2);
	float CalcLineLineDist(const Line& line1, const Line& line2);

	// 2�����̍ŒZ����
	// seg1 : ����1
	// seg1 : ����2
	// p1 : S1���̐����̑��i�߂�l�j
	// p2 : S2���̐����̑��i�߂�l�j
	// t1 : S1���̃x�N�g���W���i�߂�l�j
	// t2 : S2���̃x�N�g���W���i�߂�l�j
	// �߂�l : �ŒZ����
	float CalcSegmentSegmentDist(const Segment& seg1, const Segment& seg2, Vector3& p1, Vector3& p2, float& t1, float& t2);
	float CalcSegmentSegmentDist(const Segment& seg1, const Segment& seg2);
}