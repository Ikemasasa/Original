 #pragma once
#include <Windows.h>
#include <directXmath.h>

class Matrix;

class Vector2
{
public:
	static const Vector2 ZERO;
	static const Vector2 ONE;

public:
	float x, y;

public:
	Vector2() = default;
	Vector2(float x, float y) { this->x = x; this->y = y; }
	~Vector2() = default;

	// operator
	inline bool operator == (const Vector2& r) const { return x == r.x && y == r.y; }
	inline bool operator != (const Vector2& r) const { return x != r.x || y != r.y; }
	inline Vector2 operator -() const { return Vector2(-x, -y); }

	inline Vector2 operator +(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	inline Vector2 operator -(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	inline Vector2 operator *(const float r) const { return Vector2(x * r, y * r); }
	inline Vector2 operator /(const float r) const { return Vector2(x / r, y / r); }
	inline void operator +=(const Vector2& v) { x += v.x; y += v.y; }
	inline void operator -=(const Vector2& v) { x -= v.x; y -= v.y; }
	inline void operator *=(const float f) { x *= f; y *= f; }
	inline void operator /=(const float f)  { x /= f; y /= f; }

	// ŒvŽZ
	static float Dot(const Vector2& v0, const Vector2& v1);
	static float Cross(const Vector2& v0, const Vector2& v1);

	// ’·‚³
	float Length() const;
	float LengthSq() const;

	// ³‹K‰»
	void Normalize();
	Vector2 GetNormalize() const;
};

class Vector3
{
public:
	static const Vector3 ZERO;
	static const Vector3 ONE;

public:
	float x, y, z;

public:
	Vector3() = default;
	Vector3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	~Vector3() = default;

	inline bool operator == (const Vector3& r) const { return x == r.x && y == r.y && z == r.z; }
	inline bool operator != (const Vector3& r) const { return x != r.x || y != r.y || z != r.z; }
	inline Vector3 operator -() const { return Vector3(-x, -y, -z); }

	inline Vector3 operator +(const Vector3& r) const { return Vector3(x + r.x, y + r.y, z + r.z); }
	inline Vector3 operator -(const Vector3& r) const { return Vector3(x - r.x, y - r.y, z - r.z); }
	inline Vector3 operator +(const float r)  const { return Vector3(x + r, y + r, z + r); }
	inline Vector3 operator -(const float r)  const { return Vector3(x - r, y - r, z - r); }
	inline Vector3 operator *(const float r)  const { return Vector3(x * r, y * r, z * r); }
	inline Vector3 operator /(const float r)  const { return Vector3(x / r, y / r, z / r); }

	inline void operator +=(const Vector3& r) { x += r.x, y += r.y, z += r.z; }
	inline void operator -=(const Vector3& r) { x -= r.x, y -= r.y, z -= r.z; }
	inline void operator /=(const float r) { x /= r, y /= r, z /= r; }
	inline void operator *=(const float r) { x *= r, y *= r, z *= r; }

	static float Dot(const Vector3& v0, const Vector3& v1);
	static Vector3 Cross(const Vector3& v0, const Vector3& v1);
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	static Vector3 Transform(const Vector3& v, const Matrix& m);
	static Vector3 TransformCoord(const Vector3& v, const Matrix& m);

	//’·‚³
	float Length() const;
	float LengthSq() const;

	//³‹K‰»
	void Normalize();
	Vector3 GetNormalize() const;

	// À•W•ÏŠ·
	Vector2 WorldToScreen(const Matrix& view, const Matrix& proj) const;
};

class Vector4 
{
public:
	static const Vector4 ZERO;
	static const Vector4 ONE;

public:
	float x, y, z, w;

public:
	Vector4() = default;
	Vector4(float x, float y, float z, float w)
	{
		this->x = x; this->y = y; this->z = z; this->w = w;
	}
	Vector4(const Vector3& rgb, float a)
	{
		this->x = rgb.x; this->y = rgb.y; this->z = rgb.z; this->w = a;
	}
	~Vector4() = default;

	inline BOOL operator == (const Vector4& r) const { return x == r.x && y == r.y && z == r.z; }
	inline BOOL operator != (const Vector4& r) const { return x != r.x || y != r.y || z != r.z; }

	inline Vector4 operator +(const Vector4& r) { return Vector4(x + r.x, y + r.y, z + r.z, w + r.w); }
	inline Vector4 operator -(const Vector4& r) { return Vector4(x - r.x, y - r.y, z - r.z, w - r.w); }
	inline Vector4 operator *(const float r)  const { return Vector4(x * r, y * r, z * r, w * r); }
	inline Vector4 operator /(const float r)  const { return Vector4(x / r, y / r, z / r, w / r); }

	inline void operator +=(const Vector4& r) { x += r.x, y += r.y, z += r.z, w += r.w; }
	inline void operator -=(const Vector4& r) { x -= r.x, y -= r.y, z -= r.z, w -= r.w; }
};