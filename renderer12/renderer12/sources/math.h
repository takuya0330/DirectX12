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

struct Matrix
{
	union
	{
		float4 r[4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	constexpr Matrix()noexcept :
		_11(0), _12(0), _13(0), _14(0),
		_21(0), _22(0), _23(0), _24(0),
		_31(0), _32(0), _33(0), _34(0),
		_41(0), _42(0), _43(0), _44(0) {}

	constexpr Matrix(
		float _m11, float _m12, float _m13, float _m14,
		float _m21, float _m22, float _m23, float _m24,
		float _m31, float _m32, float _m33, float _m34,
		float _m41, float _m42, float _m43, float _m44)noexcept :
		_11(_m11), _12(_m12), _13(_m13), _14(_m14),
		_21(_m21), _22(_m22), _23(_m23), _24(_m24),
		_31(_m31), _32(_m32), _33(_m33), _34(_m34),
		_41(_m41), _42(_m42), _43(_m43), _44(_m44) {}

	constexpr Matrix(const float4& _r1, const float4& _r2, const float4& _r3, const float4& _r4)noexcept :
		_11(_r1.x), _12(_r1.y), _13(_r1.z), _14(_r1.w),
		_21(_r2.x), _22(_r2.y), _23(_r2.z), _24(_r2.w),
		_31(_r3.x), _32(_r3.y), _33(_r3.z), _34(_r3.w),
		_41(_r4.x), _42(_r4.y), _43(_r4.z), _44(_r4.w) {}

	constexpr Matrix(const Matrix& _m)noexcept :
		_11(_m._11), _12(_m._12), _13(_m._13), _14(_m._14),
		_21(_m._21), _22(_m._22), _23(_m._23), _24(_m._24),
		_31(_m._31), _32(_m._32), _33(_m._33), _34(_m._34),
		_41(_m._41), _42(_m._42), _43(_m._43), _44(_m._44) {}

	Matrix(const DirectX::XMMATRIX& _m)noexcept :
		_11(_m.r[0].m128_f32[0]), _12(_m.r[0].m128_f32[1]), _13(_m.r[0].m128_f32[2]), _14(_m.r[0].m128_f32[3]),
		_21(_m.r[1].m128_f32[0]), _22(_m.r[1].m128_f32[1]), _23(_m.r[1].m128_f32[2]), _24(_m.r[1].m128_f32[3]),
		_31(_m.r[2].m128_f32[0]), _32(_m.r[2].m128_f32[1]), _33(_m.r[2].m128_f32[2]), _34(_m.r[2].m128_f32[3]),
		_41(_m.r[3].m128_f32[0]), _42(_m.r[3].m128_f32[1]), _43(_m.r[3].m128_f32[2]), _44(_m.r[3].m128_f32[3]) {}

	operator DirectX::XMMATRIX()const noexcept { return { _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44 }; }

	constexpr bool operator==(const Matrix& _m)const noexcept { return r[0] == _m.r[0] && r[1] == _m.r[1] && r[2] == _m.r[2] && r[3] == _m.r[3]; }

	constexpr bool operator!=(const Matrix& _m)const noexcept { return r[0] != _m.r[0] || r[1] != _m.r[1] || r[2] != _m.r[2] || r[3] != _m.r[3]; }

	Matrix operator*(const Matrix& _m)const { return DirectX::XMMatrixMultiply(*this, _m); }

	Matrix Inverse()const { return DirectX::XMMatrixInverse(nullptr, *this); }

	Matrix Transpose()const { return DirectX::XMMatrixTranspose(*this); }

	static Matrix Scale(const float3& _Scale) { return DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z); }

	static Matrix Rotate(const float3& _Radian) { return DirectX::XMMatrixRotationRollPitchYaw(_Radian.x, _Radian.y, _Radian.z); }

	static Matrix Translation(const float3& _Position) { return DirectX::XMMatrixTranslation(_Position.x, _Position.y, _Position.z); }
};

inline float3 operator*(const float3& _v, const Matrix& _m)
{
	const DirectX::XMVECTOR v = DirectX::XMVector3TransformCoord({ _v.x, _v.y, _v.z, 1.0f }, _m);
	return { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
}

inline float4 operator*(const float4& _v, const Matrix& _m)
{
	const DirectX::XMVECTOR v = DirectX::XMVector4Transform({ _v.x, _v.y, _v.z, _v.w }, _m);
	return { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2], v.m128_f32[3] };
}

namespace math
{
	static constexpr float PI = 3.14159265359f;

	inline constexpr float ToRadian(float _Degree)noexcept { return _Degree * (PI / 180.0f); }

	inline constexpr float ToDegree(float _Radian)noexcept { return _Radian * (180.0f / PI); }

	template<class T>
	inline constexpr T Lerp(T _a, T _b, T _f)noexcept { return _a + _f * (_b - _a); }

	template<class T>
	inline constexpr T Clamp(T _V, T _Min, T _Max)noexcept { return (_Min <= _V && _V <= _Max) ? _V : (_V < _Min) ? _Min : _Max; }
};