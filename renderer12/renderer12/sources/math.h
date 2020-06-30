#pragma once
#include <cmath>

template<class Type>
struct Type2
{
	Type x, y;
	Type2() : x(0), y(0) {}

	constexpr Type2(Type _x, Type _y)noexcept : x(_x), y(_y) {}
	
	constexpr Type2(Type _xy)noexcept : x(_xy), y(_xy) {}
	
	constexpr Type2(const Type2& _v)noexcept : x(_v.x), y(_v.y) {}

	constexpr bool operator==(const Type2& _v)const noexcept { return x == _v.x && y == _v.y; }
	
	constexpr bool operator!=(const Type2& _v)const noexcept { return x != _v.x && y != _v.y; }

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

	constexpr Type Length()const noexcept { return std::sqrt(LengthSq()); }

	constexpr Type LengthSq()const noexcept { return x * x + y * y; }

	constexpr Type2 Normalize()const noexcept { return *this / Length(); }

	template<class T>
	constexpr auto Dot(const Type2<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.x + y * _v.y; }

	template<class T>
	constexpr auto Cross(const Type2<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.y - y * _v.x; }
};

template<class Type>
struct Type3
{
	Type x, y, z;
	Type3() : x(0), y(0), z(0) {}

	constexpr Type3(Type _x, Type _y, Type _z)noexcept : x(_x), y(_y), z(_z) {}
	
	constexpr Type3(Type _v)noexcept : x(_v), y(_v), z(_v) {}

	constexpr bool operator==(const Type3& _v)const noexcept { return x == _v.x && y == _v.y && z == _v.z; }
	
	constexpr bool operator!=(const Type3& _v)const noexcept { return x != _v.x && y != _v.y && z != _v.z; }

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
	constexpr auto Dot(const Type3<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.x + y * _v.y + z * _v.z; }

	template<class T>
	constexpr auto Cross(const Type3<T>& _v)const noexcept->Type3<decltype(x* _v.x)> { return{ y * _v.z - z * _v.y, z * _v.x - x * _v.z, x * _v.y - y * _v.x }; }
};

template<class Type>
struct Type4
{
	Type x, y, z, w;
	Type4() : x(0), y(0), z(0), w(0) {}

	constexpr Type4(Type _x, Type _y, Type _z, Type _w)noexcept : x(_x), y(_y), z(_z), w(_w) {}
	constexpr Type4(Type _v)noexcept : x(_v), y(_v), z(_v), w(_v) {}

	constexpr bool operator==(const Type4& _v)const noexcept { return x == _v.x && y == _v.y && z == _v.z && w == _v.w; }
	constexpr bool operator!=(const Type4& _v)const noexcept { return x != _v.x && y != _v.y && z != _v.z && w != _v.w; }

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
};

using int2 = Type2<int>;
using int3 = Type3<int>;
using int4 = Type4<int>;
using float2 = Type2<float>;
using float3 = Type3<float>;
using float4 = Type4<float>;