#pragma once
#include "Misc/Types.h"

using int2 = Type2<int>;
using int3 = Type3<int>;
using int4 = Type4<int>;
using uint = unsigned int;
using uint64 = unsigned long long;
using uint2 = Type2<uint>;
using uint3 = Type3<uint>;
using uint4 = Type4<uint>;
using float2 = Type2<float>;
using float3 = Type3<float>;
using float4 = Type4<float>;
//using Vector = DirectX::XMVECTOR;

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

	static Matrix Scale(const float3& scale) { return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z); }

	static Matrix Rotate(const float3& radian) { return DirectX::XMMatrixRotationRollPitchYaw(radian.x, radian.y, radian.z); }

	static Matrix Translation(const float3& position) { return DirectX::XMMatrixTranslation(position.x, position.y, position.z); }

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