#include "Vector.h"
#include "Framework.h"

#include "Math.h"
#include "Matrix.h"

// staticメンバ変数
const Vector2 Vector2::ZERO = Vector2(0, 0);
const Vector2 Vector2::ONE  = Vector2(1, 1);
const Vector3 Vector3::ZERO = Vector3(0, 0, 0);
const Vector3 Vector3::ONE  = Vector3(1, 1, 1);
const Vector4 Vector4::ZERO = Vector4(0, 0, 0, 0);
const Vector4 Vector4::ONE  = Vector4(1, 1, 1, 1);

//---------------------------------------------------
// Vector2
//---------------------------------------------------
float Vector2::Dot(const Vector2& v0, const Vector2& v1)
{
	return v0.x * v1.x + v0.y * v1.y;
}

float Vector2::Cross(const Vector2& v0, const Vector2& v1)
{
	return v0.x * v1.y - v0.y * v1.x;
}

float Vector2::Length() const
{
	return sqrtf(x * x + y * y);
}

float Vector2::LengthSq() const
{
	return x * x + y * y;
}

void Vector2::Normalize()
{
	float len = sqrtf(x * x + y * y);
	x /= len;
	y /= len;
}

Vector2 Vector2::GetNormalize() const
{
	float len = sqrtf(x * x + y * y);
	Vector2 normalize;
	normalize.x = x / len;
	normalize.y = y / len;
	return normalize;
}

//---------------------------------------------------
// Vector3
//---------------------------------------------------
float Vector3::Dot(const Vector3& v0, const Vector3& v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

Vector3 Vector3::Cross(const Vector3& v0, const Vector3& v1)
{
	Vector3 cross;
	cross.x = v0.y * v1.z - v0.z * v1.y;
	cross.y = v0.z * v1.x - v0.x * v1.z;
	cross.z = v0.x * v1.y - v0.y * v1.x;
	return cross;
}

Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	t = Math::Clamp01(t);

	Vector3 ret;
	ret.x = v1.x + t * (v2.x - v1.x);
	ret.y = v1.y + t * (v2.y - v1.y);
	ret.z = v1.z + t * (v2.z - v1.z);
	return ret;
}

Vector3 Vector3::Transform(const Vector3& v, const Matrix& m)
{
	Vector3 ret;
	ret.x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
	ret.y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
	ret.z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;
	return ret;
}

Vector3 Vector3::TransformCoord(const Vector3& v, const Matrix& m)
{
	Vector3 ret;
	float w = v.x * m._14 + v.y * m._24 + v.z * m._34 + m._44;
	ret.x = (v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41) / w;
	ret.y = (v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42) / w;
	ret.z = (v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43) / w;

	return ret;
}

float Vector3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector3::LengthSq() const
{
	return x * x + y * y + z * z;
}

void Vector3::Normalize()
{
	float len = sqrtf(x * x + y * y + z * z);
	x /= len;
	y /= len;
	z /= len;
}

Vector3 Vector3::GetNormalize() const
{
	float len = sqrtf(x * x + y * y + z * z);
	Vector3 normalize;
	normalize.x = x / len;
	normalize.y = y / len;
	normalize.z = z / len;
	return normalize;
}

Vector2 Vector3::WorldToScreen(const Matrix& view, const Matrix& proj) const
{
	Vector3 pos = *this;
	
	// 座標変換
	pos = Vector3::TransformCoord(pos, view);
	pos = Vector3::TransformCoord(pos, proj);

	// ビューポート取得
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();
	D3D11_VIEWPORT vp;
	UINT numViewport = 1;
	context->RSGetViewports(&numViewport, &vp);

	// ビューポート行列作成
	float w = vp.Width * 0.5f;
	float h = vp.Height * 0.5f;
	Matrix viewport;
	viewport._11 = w; viewport._12 = 0;  viewport._13 = 0; viewport._14 = 0;
	viewport._21 = 0; viewport._22 = -h; viewport._23 = 0; viewport._24 = 0;
	viewport._31 = 0; viewport._32 = 0;  viewport._33 = 1; viewport._34 = 0;
	viewport._41 = w; viewport._42 = h;  viewport._43 = 0; viewport._44 = 1;

	// スクリーン座標変換
	pos = Vector3::TransformCoord(pos, viewport);
	return Vector2(pos.x, pos.y);
}