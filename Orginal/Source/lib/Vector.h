 #pragma once
#include <Windows.h>
#include <directXmath.h>

class Vector2 : public DirectX::XMFLOAT2
{
public:
	static const Vector2 ZERO;
	static const Vector2 ONE;

public:
	Vector2() = default;
	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	Vector2(const DirectX::XMVECTOR& org) : XMFLOAT2()
	{
		DirectX::XMVECTOR temp = org;
		DirectX::XMStoreFloat2(this, temp);
	}
	~Vector2() = default;


	inline bool operator == (const Vector2& r) const { return x == r.x && y == r.y; }
	inline bool operator != (const Vector2& r) const { return x != r.x || y != r.y; }
	inline Vector2 operator *(const float r) const { return Vector2(x * r, y * r); }
	inline Vector2 operator /(const float r) const { return Vector2(x / r, y / r); }

	// ベクトルの内積
	float Dot(const Vector2& In) const { return x * In.x + y * In.y; }

	//代入
	Vector2& operator=(const DirectX::XMVECTOR& other)
	{
		DirectX::XMVECTOR temp = other;
		XMStoreFloat2(this, temp);
		return *this;
	}

	//キャスト
	operator DirectX::XMVECTOR() const {
		return XMLoadFloat2(this);
	}

	//長さ
	float Length() const
	{
		return (static_cast<Vector2>(DirectX::XMVector2Length(DirectX::XMVECTOR(*this)))).x;
	}
	float LengthSq() const
	{
		return (static_cast<Vector2>(DirectX::XMVector2LengthSq(DirectX::XMVECTOR(*this)))).x;
	}

	//正規化
	void Normalize() { *this = DirectX::XMVector2Normalize(DirectX::XMVECTOR(*this)); }
	Vector2 GetNormalize(){ return DirectX::XMVector2Normalize(DirectX::XMVECTOR(*this)); }
};

class Vector3 : public DirectX::XMFLOAT3
{
public:
	static const Vector3 ZERO;
	static const Vector3 ONE;

public:
	Vector3() = default;
	Vector3(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
	}
	Vector3(const DirectX::XMVECTOR& other) :XMFLOAT3() {
		DirectX::XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
	}
	~Vector3() = default;

	inline BOOL operator == (const Vector3& r) const { return x == r.x && y == r.y && z == r.z; }
	inline BOOL operator != (const Vector3& r) const { return x != r.x || y != r.y || z != r.z; }

	//inline Vector3 operator +(const Vector3& r) const { return Vector3(x + r.x, y + r.y, z + r.z); }
	//inline Vector3 operator -(const Vector3& r) const { return Vector3(x - r.x, y - r.y, z - r.z); }

	inline Vector3 operator +(const float r)  const { return Vector3(x + r, y + r, z + r); }
	inline Vector3 operator -(const float r)  const { return Vector3(x - r, y - r, z - r); }
	inline Vector3 operator *(const float r)  const { return Vector3(x * r, y * r, z * r); }
	inline Vector3 operator /(const float r)  const { return Vector3(x / r, y / r, z / r); }

	inline void operator +=(const Vector3& r) { x += r.x, y += r.y, z += r.z; }
	inline void operator -=(const Vector3& r) { x -= r.x, y -= r.y, z -= r.z; }

	inline void operator /=(const float r) { x /= r, y /= r, z /= r; }
	inline void operator *=(const float r) { x *= r, y *= r, z *= r; }

	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	// ベクトルの内積
	static float Dot(const Vector3& v0, const Vector3& v1) { return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; }

	// ベクトルの外積
	static Vector3 Cross(const Vector3& v0, const Vector3& v1) { return Vector3(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x); }

	//代入
	Vector3& operator=(const DirectX::XMVECTOR & other) 
	{
		DirectX::XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
		return *this;
	}

	//キャスト
	operator DirectX::XMVECTOR() const
	{
		return XMLoadFloat3(this);
	}

	//長さ
	float Length() const 
	{
		return (static_cast<Vector3>(DirectX::XMVector3Length(DirectX::XMVECTOR(*this)))).x;
	}
	float LengthSq() const
	{
		return (static_cast<Vector3>(DirectX::XMVector3LengthSq(DirectX::XMVECTOR(*this)))).x;
	}

	//正規化
	void Normalize() { *this = DirectX::XMVector3Normalize(DirectX::XMVECTOR(*this)); }
	Vector3 GetNormalize() { return DirectX::XMVector3Normalize(DirectX::XMVECTOR(*this)); }

	// 座標変換
	Vector2 WorldToScreen(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj) const;
};

class Vector4 : public DirectX::XMFLOAT4
{
public:
	static const Vector4 ZERO;
	static const Vector4 ONE;

public:
	Vector4() = default;
	Vector4(float x, float y, float z, float w)
	{
		this->x = x; this->y = y; this->z = z; this->w = w;
	}
	Vector4(const DirectX::XMVECTOR& other) : XMFLOAT4() {
		DirectX::XMVECTOR temp = other;
		XMStoreFloat4(this, temp);
	}
	~Vector4() = default;

	inline BOOL operator == (const Vector4& r) const { return x == r.x && y == r.y && z == r.z; }
	inline BOOL operator != (const Vector4& r) const { return x != r.x || y != r.y || z != r.z; }
	inline Vector4 operator *(const float r)  const { return Vector4(x * r, y * r, z * r, w * r); }
	inline Vector4 operator /(const float r)  const { return Vector4(x / r, y / r, z / r, w / r); }
	inline Vector4 operator +=(const Vector4& r) { return Vector4(x += r.x, y += r.y, z += r.z, w += r.w); }
	inline Vector4 operator -=(const Vector4& r) { return Vector4(x -= r.x, y -= r.y, z -= r.z, w -= r.w); }

	//代入
	Vector4& operator=(const DirectX::XMVECTOR& other)
	{
		DirectX::XMVECTOR temp = other;
		XMStoreFloat4(this, temp);
		return *this;
	}

	//キャスト
	operator DirectX::XMVECTOR() const
	{
		return XMLoadFloat4(this);
	}
	
	//正規化
	void Normalize()
	{
		*this = DirectX::XMVector4Normalize(DirectX::XMVECTOR(*this));
	}
};

class Matrix
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[16];
	};

	float* Data() { return m; }

	void Identity();
	void Multiply(const Matrix& mat1, const Matrix& mat2);
	void Multiply(float val);
	void Inverse();


	Matrix& operator *=(const Matrix& mat)
	{
		Multiply(*this, mat);
		return *this;
	}

	Matrix operator *(const Matrix& mat) const
	{
		Matrix m;
		m.Multiply(*this, mat);
		return m;
	}

	Matrix operator +(const Matrix& mat) const
	{
		Matrix m;
		for (int i = 0; i < 16; ++i)
		{
			m.m[i] = this->m[i] + mat.m[i];
		}
		return m;
	}

	Matrix operator *(float val) const
	{
		Matrix m;
		for (int i = 0; i < 16; ++i)
		{
			m.m[i] = this->m[i] * val;
		}
		return m;
	}
};