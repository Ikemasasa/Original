#pragma once

// 単に複数の変数を持つよう
// オペレーターがちょいとあるくらい

template <class T>
class TVector2
{
public:
	T x;
	T y;

public:
	TVector2() : x(NULL), y(NULL) {}
	TVector2(const T& x, const T& y)
	{
		this->x = x;
		this->y = y;
	}
	TVector2(const TVector2<T>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}
	~TVector2() = default;

	inline bool operator == (const TVector2<T>& v) const { return x == v.x && y == v.y; }
	inline bool operator != (const TVector2<T>& v) const { return x != v.x || y != v.y; }

	inline TVector2<T>& operator +(const T& v)  const { return TVector2<T>(x + v, y + v); }
	inline TVector2<T>& operator -(const T& v)  const { return TVector2(x - v, y - v); }
	inline TVector2<T>& operator *(const T& v)  const { return TVector2(x * v, y * v); }
	inline TVector2<T>& operator /(const T& v)  const { return TVector2(x / v, y / v); }

	inline void	operator +=(const TVector2<T>& v) { x += v.x, y += v.y; }
	inline void	operator -=(const TVector2<T>& v) { x -= v.x, y -= v.y; }

	inline void	operator +=(const T& v) { x += v, y += v; }
	inline void	operator -=(const T& v) { x -= v, y -= v; }
	inline void operator *=(const T& v) { x *= v, y *= v; }
	inline void operator /=(const T& v) { x /= v, y /= v; }
};

template <class T>
class TVector3
{
public:
	T x;
	T y;
	T z;

public:
	TVector3() : x(NULL), y(NULL), z(NULL) {}
	TVector3(const T& x, const T& y, const T& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	TVector3(const TVector3<T>& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}
	~TVector3() = default;

	inline bool operator == (const TVector3<T>& v) const { return x == v.x && y == v.y && z == v.z; }
	inline bool operator != (const TVector3<T>& v) const { return x != v.x || y != v.y || z != v.z; }

	inline TVector3<T>& operator +(const T& v)  const { return TVector3(x + v, y + v, z + v); }
	inline TVector3<T>& operator -(const T& v)  const { return TVector3(x - v, y - v, z - v); }
	inline TVector3<T>& operator *(const T& v)  const { return TVector3(x * v, y * v, z * v); }
	inline TVector3<T>& operator /(const T& v)  const { return TVector3(x / v, y / v, z / v); }

	inline void	operator +=(const TVector3<T>& v) { x += v.x, y += v.y, z += v.z; }
	inline void	operator -=(const TVector3<T>& v) { x -= v.x, y -= v.y, z -= v.z; }

	inline void	operator +=(const T& v) { x += v, y += v, z += v; }
	inline void	operator -=(const T& v) { x -= v, y -= v, z -= v; }
	inline void operator *=(const T& v) { x *= v, y *= v, z *= v; }
	inline void operator /=(const T& v) { x /= v, y /= v, z /= v; }
};