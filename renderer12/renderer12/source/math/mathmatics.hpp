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

	[[nodiscard]] constexpr Type2 Radian()const noexcept { return { x * 0.01745f, y * 0.01745f }; }

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

	[[nodiscard]] constexpr Type3 Radian()const noexcept { return { x * 0.01745f, y * 0.01745f, z * 0.01745f }; }

	[[nodiscard]] constexpr DirectX::XMVECTOR Convert(Type _w)const noexcept { return DirectX::XMVectorSet(x, y, z, _w); }

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

#pragma region subscribe
using int2 = Type2<int>;
using int3 = Type3<int>;
using int4 = Type4<int>;
using uint = unsigned int;
using uint2 = Type2<uint>;
using uint3 = Type3<uint>;
using uint4 = Type4<uint>;
using float2 = Type2<float>;
using float3 = Type3<float>;
using float4 = Type4<float>;
//using Vector = DirectX::XMVECTOR;
#pragma endregion 

struct Matrix
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
		float4 r[4];
	};

	constexpr Matrix()noexcept :
		_11(0), _12(0), _13(0), _14(0),
		_21(0), _22(0), _23(0), _24(0),
		_31(0), _32(0), _33(0), _34(0),
		_41(0), _42(0), _43(0), _44(0) {}

	constexpr Matrix(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44)noexcept :
		_11(m11), _12(m12), _13(m13), _14(m14),
		_21(m21), _22(m22), _23(m23), _24(m24),
		_31(m31), _32(m32), _33(m33), _34(m34),
		_41(m41), _42(m42), _43(m43), _44(m44) {}

	constexpr Matrix(const float4& r1, const float4& r2, const float4& r3, const float4& r4)noexcept :
		_11(r1.x), _12(r1.y), _13(r1.z), _14(r1.w),
		_21(r2.x), _22(r2.y), _23(r2.z), _24(r2.w),
		_31(r3.x), _32(r3.y), _33(r3.z), _34(r3.w),
		_41(r4.x), _42(r4.y), _43(r4.z), _44(r4.w) {}

	constexpr Matrix(const Matrix& m)noexcept :
		_11(m._11), _12(m._12), _13(m._13), _14(m._14),
		_21(m._21), _22(m._22), _23(m._23), _24(m._24),
		_31(m._31), _32(m._32), _33(m._33), _34(m._34),
		_41(m._41), _42(m._42), _43(m._43), _44(m._44) {}

	Matrix(const DirectX::XMMATRIX& m)noexcept :
		_11(m.r[0].m128_f32[0]), _12(m.r[0].m128_f32[1]), _13(m.r[0].m128_f32[2]), _14(m.r[0].m128_f32[3]),
		_21(m.r[1].m128_f32[0]), _22(m.r[1].m128_f32[1]), _23(m.r[1].m128_f32[2]), _24(m.r[1].m128_f32[3]),
		_31(m.r[2].m128_f32[0]), _32(m.r[2].m128_f32[1]), _33(m.r[2].m128_f32[2]), _34(m.r[2].m128_f32[3]),
		_41(m.r[3].m128_f32[0]), _42(m.r[3].m128_f32[1]), _43(m.r[3].m128_f32[2]), _44(m.r[3].m128_f32[3]) {}

	operator DirectX::XMMATRIX()const noexcept { return { _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44 }; }

	constexpr bool operator==(const Matrix& m)const noexcept { return r[0] == m.r[0] && r[1] == m.r[1] && r[2] == m.r[2] && r[3] == m.r[3]; }

	constexpr bool operator!=(const Matrix& m)const noexcept { return r[0] != m.r[0] || r[1] != m.r[1] || r[2] != m.r[2] || r[3] != m.r[3]; }

	Matrix operator=(const Matrix& m)noexcept {
		_11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
		_21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
		_31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
		_41 = m._41; _42 = m._42; _43 = m._43; _44 = m._44;
		return *this;
	}

	Matrix operator*(const Matrix& m)const { return DirectX::XMMatrixMultiply(*this, m); }

	Matrix Inverse()const { return DirectX::XMMatrixInverse(nullptr, *this); }

	Matrix Transpose()const { return DirectX::XMMatrixTranspose(*this); }

	static Matrix Scale(const float3& _Scale) { return DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z); }

	static Matrix Rotate(const float3& _Radian) { return DirectX::XMMatrixRotationRollPitchYaw(_Radian.x, _Radian.y, _Radian.z); }

	static Matrix Rotate(const float3& _Axis, float _Radian) { return DirectX::XMMatrixRotationAxis(_Axis.Convert(0), _Radian); }

	static Matrix Translation(const float3& _Position) { return DirectX::XMMatrixTranslation(_Position.x, _Position.y, _Position.z); }

	static Matrix AffineTransformation2D(const float4& _Scale, const float4& _Rotate, float _Angle, const float4& _Position) { return DirectX::XMMatrixAffineTransformation2D(_Scale, _Rotate, _Angle, _Position); }
};

inline float3 operator*(const float3& v, const Matrix& m)
{
	const DirectX::XMVECTOR value = DirectX::XMVector3TransformCoord({ v.x, v.y, v.z, 1.0f }, m);
	return { value.m128_f32[0], value.m128_f32[1], value.m128_f32[2] };
}

inline float4 operator*(const float4& v, const Matrix& m)
{
	const DirectX::XMVECTOR value = DirectX::XMVector4Transform({ v.x, v.y, v.z, v.w }, m);
	return { value.m128_f32[0], value.m128_f32[1], value.m128_f32[2], value.m128_f32[3] };
}