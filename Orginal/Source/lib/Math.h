#pragma once
#include "Vector.h"

#define _OX_EPSILON_	0.000001f	// 誤差

struct Line
{
	Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
	Line(const Vector3& p, const Vector3& v) : p(p), v(v) {}

	Vector3 p; // 始点
	Vector3 v; // 方向ベクトル

	// 点上の座標を取得
	//  ベクトルに掛け算する係数
	Vector3 GetPoint(float t) const { return p + t * v; }
};

struct Segment : public Line
{
	Segment() : Line() {}
	Segment(const Vector3& p, const Vector3& v) : Line(p, v) {}

	// これをいれてもよき
	// Vector3 len; // 方向ベクトルの長さ
	
	// 終点を獲得
	Vector3 GetEndPoint() const { return p + v; }
};

class Math
{
private:
	static bool IsSharpAngle(const Vector3& p1, const Vector3& p2, const Vector3& p3);	// ∠p1p2p3 が鋭角(0～90度)か
	static bool IsParallel(const Vector3& v1, const Vector3& v2); // 二本のベクトルが平行か

public:
	static float Max(const float& c1, const float& c2);
	static int Max(const int& c1, const int& c2);
	static float Min(const float& c1, const float& c2);
	static int Min(const int& c1, const int& c2);
	static float Clamp(const float& value, const float& min, const float& max);
	static float Clamp(const int& value, const int& min, const int& max);
	static float Clamp01(const float& value);
	static float Lerp(const float& f1, const float& f2, const float& t);

	// 点と直線の最短距離
	// p	: 点
	// line : 直線
	// h	: 点から降ろした垂線の足
	// t	: ベクトル係数
	// 戻り値 : 最短距離
	static float CalcPointLineDist(const Vector3& p, const Line& line, Vector3& h, float& t);
	static float CalcPointLineDist(const Vector3& p, const Line& line);
	
	// 点と線分の最短距離
	// p   : 点
	// seg : 線分
	// h   : 最短距離となる端点
	// t   : 端点位置(　t < 0: 始点の外, 0 <= t <= 1: 線分内, t > 1: 終点の外　)
	// 戻り値 : 最短距離
	static float CalcPointSegmentDist(const Vector3& p, const Segment& seg, Vector3& h, float& t);
	static float CalcPointSegmentDist(const Vector3& p, const Segment& seg);

	// 2直線の最短距離
	// line1 : 直線１
	// line2 : 直線２
	// p1 : L1側の垂線の足（戻り値）
	// p2 : L2側の垂線の足（戻り値）
	// t1 : L1側のベクトル係数（戻り値）
	// t2 : L2側のベクトル係数（戻り値）
	// 戻り値 : 最短距離
	static float CalcLineLineDist(const Line& line1, const Line& line2, Vector3& p1, Vector3& p2, float& t1, float& t2);
	static float CalcLineLineDist(const Line& line1, const Line& line2);

	// 2線分の最短距離
	// seg1 : 線分1
	// seg1 : 線分2
	// p1 : S1側の垂線の足（戻り値）
	// p2 : S2側の垂線の足（戻り値）
	// t1 : S1側のベクトル係数（戻り値）
	// t2 : S2側のベクトル係数（戻り値）
	// 戻り値 : 最短距離
	static float CalcSegmentSegmentDist(const Segment& seg1, const Segment& seg2, Vector3& p1, Vector3& p2, float& t1, float& t2);
	static float CalcSegmentSegmentDist(const Segment& seg1, const Segment& seg2);
};