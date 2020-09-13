#pragma once
#include <cmath>
#include <DirectXMath.h>

template<class Type>
struct Type2
{
	Type x, y;
	Type2() : x(0), y(0) {}

	constexpr Type2(Type _x, Type _y)noexcept : x(_x), y(_y) {}
	constexpr Type2(Type _xy)noexcept : x(_xy), y(_xy) {}
	constexpr Type2(const Type2& _v)noexcept : x(_v.x), y(_v.y) {}

	//template<class Cast>
	//constexpr operator Cast()const noexcept { return static_cast<Cast>(x, y); }

	[[nodiscard]] constexpr bool operator==(const Type2& _v)const noexcept { return x == _v.x && y == _v.y; }
	[[nodiscard]] constexpr bool operator!=(const Type2& _v)const noexcept { return x != _v.x && y != _v.y; }
	//[[nodiscard]] constexpr bool operator<=>(const Type2& _v)const noexcept = default;
	[[nodiscard]] constexpr Type2 operator+()const noexcept { return *this; }
	[[nodiscard]] constexpr Type2 operator-()const noexcept { return { -x, -y }; }

	constexpr Type2 operator+(const Type2& _v)const noexcept { return { x + _v.x, y + _v.y }; }
	constexpr Type2 operator-(const Type2& _v)const noexcept { return { x - _v.x, y - _v.y }; }
	constexpr Type2 operator*(const Type2& _v)const noexcept { return { x * _v.x, y * _v.y }; }
	constexpr Type2 operator/(const Type2& _v)const noexcept { return { x / _v.x, y / _v.y }; }

	Type2 operator=(const Type2& _v)noexcept { x = _v.x; y = _v.y; return *this; }
	Type2 operator+=(const Type2& _v)noexcept { x += _v.x; y += _v.y; return *this; }
	Type2 operator-=(const Type2& _v)noexcept { x -= _v.x; y -= _v.y; return *this; }
	Type2 operator*=(const Type2& _v)noexcept { x *= _v.x; y *= _v.y; return *this; }
	Type2 operator/=(const Type2& _v)noexcept { x /= _v.x; y /= _v.y; return *this; }

	Type2 operator=(const Type& _v)noexcept { x = _v; y = _v; return *this; }
	Type2 operator+=(const Type& _v)noexcept { x += _v; y += _v; return *this; }
	Type2 operator-=(const Type& _v)noexcept { x -= _v; y -= _v; return *this; }
	Type2 operator*=(const Type& _v)noexcept { x *= _v; y *= _v; return *this; }
	Type2 operator/=(const Type& _v)noexcept { x /= _v; y /= _v; return *this; }

	[[nodiscard]] constexpr Type Length()const noexcept { return std::sqrt(LengthSq()); }

	[[nodiscard]] constexpr Type LengthSq()const noexcept { return x * x + y * y; }

	[[nodiscard]] constexpr Type2 Normalize()const noexcept { return *this / Length(); }

	template<class T>
	[[nodiscard]] constexpr auto Dot(const Type2<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.x + y * _v.y; }

	template<class T>
	[[nodiscard]] constexpr auto Cross(const Type2<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.y - y * _v.x; }
};

template<class Type>
struct Type3
{
	Type x, y, z;
	Type3() : x(0), y(0), z(0) {}

	//template<class Cast>
	//constexpr operator Cast()const noexcept { return static_cast<Cast>(x, y, z); }

	constexpr Type3(Type _x, Type _y, Type _z)noexcept : x(_x), y(_y), z(_z) {}
	constexpr Type3(Type _v)noexcept : x(_v), y(_v), z(_v) {}

	[[nodiscard]] constexpr bool operator==(const Type3& _v)const noexcept { return x == _v.x && y == _v.y && z == _v.z; }
	[[nodiscard]] constexpr bool operator!=(const Type3& _v)const noexcept { return x != _v.x && y != _v.y && z != _v.z; }
	//[[nodiscard]] constexpr bool operator<=>(const Type3& _v)const noexcept = default;
	[[nodiscard]] constexpr Type3 operator+()const noexcept { return *this; }
	[[nodiscard]] constexpr Type3 operator-()const noexcept { return { -x, -y, -z }; }

	constexpr Type3 operator+(const Type3& _v)const noexcept { return { x + _v.x, y + _v.y, z + _v.z }; }
	constexpr Type3 operator-(const Type3& _v)const noexcept { return { x - _v.x, y - _v.y, z - _v.z }; }
	constexpr Type3 operator*(const Type3& _v)const noexcept { return { x * _v.x, y * _v.y, z * _v.z }; }
	constexpr Type3 operator/(const Type3& _v)const noexcept { return { x / _v.x, y / _v.y, z / _v.z }; }

	Type3 operator=(const Type3& _v)noexcept { x = _v.x; y = _v.y; z = _v.z; return *this; }
	Type3 operator+=(const Type3& _v)noexcept { x += _v.x; y += _v.y; z += _v.z; return *this; }
	Type3 operator-=(const Type3& _v)noexcept { x -= _v.x; y -= _v.y; z -= _v.z; return *this; }
	Type3 operator*=(const Type3& _v)noexcept { x *= _v.x; y *= _v.y; z *= _v.z; return *this; }
	Type3 operator/=(const Type3& _v)noexcept { x /= _v.x; y /= _v.y; z /= _v.z; return *this; }

	Type3 operator=(const Type& _v)noexcept { x = _v; y = _v; z = _v; return *this; }
	Type3 operator+=(const Type& _v)noexcept { x += _v; y += _v; z += _v; return *this; }
	Type3 operator-=(const Type& _v)noexcept { x -= _v; y -= _v; z -= _v; return *this; }
	Type3 operator*=(const Type& _v)noexcept { x *= _v; y *= _v; z *= _v; return *this; }
	Type3 operator/=(const Type& _v)noexcept { x /= _v; y /= _v; z /= _v; return *this; }

	[[nodiscard]] constexpr Type Length()const noexcept { return std::sqrt(LengthSq()); }

	[[nodiscard]] constexpr Type LengthSq()const noexcept { return x * x + y * y + z * z; }

	[[nodiscard]] constexpr Type3 Normalize()const noexcept { return *this / Length(); }

	template<class T>
	[[nodiscard]] constexpr auto Dot(const Type3<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.x + y * _v.y + z * _v.z; }

	template<class T>
	[[nodiscard]] constexpr auto Cross(const Type3<T>& _v)const noexcept->Type3<decltype(x* _v.x)> { return{ y * _v.z - z * _v.y, z * _v.x - x * _v.z, x * _v.y - y * _v.x }; }
};

template<class Type>
struct Type4
{
	Type x, y, z, w;
	Type4() : x(0), y(0), z(0), w(0) {}

	constexpr Type4(Type _x, Type _y, Type _z, Type _w)noexcept : x(_x), y(_y), z(_z), w(_w) {}
	constexpr Type4(Type _v)noexcept : x(_v), y(_v), z(_v), w(_v) {}

	[[nodiscard]] constexpr bool operator==(const Type4& _v)const noexcept { return x == _v.x && y == _v.y && z == _v.z && w == _v.w; }
	[[nodiscard]] constexpr bool operator!=(const Type4& _v)const noexcept { return x != _v.x && y != _v.y && z != _v.z && w != _v.w; }
	//[[nodiscard]] constexpr bool operator<=>(const Type4& _v)const noexcept = default;
	[[nodiscard]] constexpr Type4 operator+()const noexcept { return *this; }
	[[nodiscard]] constexpr Type4 operator-()const noexcept { return { -x, -y, -z, -w }; }

	constexpr Type4 operator+(const Type4& _v)const noexcept { return { x + _v.x, y + _v.y, z + _v.z, w + _v.w }; }
	constexpr Type4 operator-(const Type4& _v)const noexcept { return { x - _v.x, y - _v.y, z - _v.z, w - _v.w }; }
	constexpr Type4 operator*(const Type4& _v)const noexcept { return { x * _v.x, y * _v.y, z * _v.z, w * _v.w }; }
	constexpr Type4 operator/(const Type4& _v)const noexcept { return { x / _v.x, y / _v.y, z / _v.z, w / _v.w }; }

	Type4 operator=(const Type4& _v)noexcept { x = _v.x; y = _v.y; z = _v.z; w = _v.w; return *this; }
	Type4 operator+=(const Type4& _v)noexcept { x += _v.x; y += _v.y; z += _v.z; w += _v.w; return *this; }
	Type4 operator-=(const Type4& _v)noexcept { x -= _v.x; y -= _v.y; z -= _v.z; w -= _v.w; return *this; }
	Type4 operator*=(const Type4& _v)noexcept { x *= _v.x; y *= _v.y; z *= _v.z; w *= _v.w; return *this; }
	Type4 operator/=(const Type4& _v)noexcept { x /= _v.x; y /= _v.y; z /= _v.z; w /= _v.w; return *this; }

	Type4 operator=(const Type& _v)noexcept { x = _v; y = _v; z = _v; w = _v; return *this; }
	Type4 operator+=(const Type& _v)noexcept { x += _v; y += _v; z += _v; w += _v; return *this; }
	Type4 operator-=(const Type& _v)noexcept { x -= _v; y -= _v; z -= _v; w -= _v; return *this; }
	Type4 operator*=(const Type& _v)noexcept { x *= _v; y *= _v; z *= _v; w *= _v; return *this; }
	Type4 operator/=(const Type& _v)noexcept { x /= _v; y /= _v; z /= _v; w /= _v; return *this; }

	operator DirectX::XMVECTOR()const noexcept { return DirectX::XMVectorSet(x, y, z, w); }
};
