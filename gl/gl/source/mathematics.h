#pragma once
#include <DirectXMath.h>
#include <cmath>

namespace gl::detail
{
	template<class type>
	struct type2
	{
		type x, y;
		type2() : x(0), y(0) {}

		constexpr type2(type x, type y)noexcept : x(x), y(y) {}

		constexpr type2(type _v)noexcept : x(_v), y(_v) {}

		constexpr type2(const type2& _v)noexcept : x(_v.x), y(_v.y) {}

		template<class T>
		constexpr type2(const type2<T>& _v)noexcept : x(static_cast<type>(_v.x)), y(static_cast<type>(_v.y)) {}

		[[nodiscard]] constexpr bool operator==(const type2& _v)const noexcept { return x == _v.x && y == _v.y; }

		[[nodiscard]] constexpr bool operator!=(const type2& _v)const noexcept { return x != _v.x && y != _v.y; }

		[[nodiscard]] constexpr bool operator>(const type2& _v)noexcept { return x > _v.x && y > _v.y; }

		[[nodiscard]] constexpr bool operator<(const type2& _v)noexcept { return x < _v.x && y < _v.y; }

		[[nodiscard]] constexpr bool operator>(const type& _v)noexcept { return x > _v && y > _v; }

		[[nodiscard]] constexpr bool operator<(const type& _v)noexcept { return x < _v&& y < _v; }

		[[nodiscard]] constexpr type2 operator+()const noexcept { return *this; }

		[[nodiscard]] constexpr type2 operator-()const noexcept { return { -x, -y }; }

		constexpr type2 operator+(const type2& _v)const noexcept { return { x + _v.x, y + _v.y }; }

		constexpr type2 operator-(const type2& _v)const noexcept { return { x - _v.x, y - _v.y }; }

		constexpr type2 operator*(const type2& _v)const noexcept { return { x * _v.x, y * _v.y }; }

		constexpr type2 operator/(const type2& _v)const noexcept { return { x / _v.x, y / _v.y }; }

		type2 operator=(const type2& _v)noexcept { x = _v.x; y = _v.y; return *this; }

		type2 operator+=(const type2& _v)noexcept { x += _v.x; y += _v.y; return *this; }

		type2 operator-=(const type2& _v)noexcept { x -= _v.x; y -= _v.y; return *this; }

		type2 operator*=(const type2& _v)noexcept { x *= _v.x; y *= _v.y; return *this; }

		type2 operator/=(const type2& _v)noexcept { x /= _v.x; y /= _v.y; return *this; }

		type2 operator=(const type& _v)noexcept { x = _v; y = _v; return *this; }

		type2 operator+=(const type& _v)noexcept { x += _v; y += _v; return *this; }

		type2 operator-=(const type& _v)noexcept { x -= _v; y -= _v; return *this; }

		type2 operator*=(const type& _v)noexcept { x *= _v; y *= _v; return *this; }

		type2 operator/=(const type& _v)noexcept { x /= _v; y /= _v; return *this; }

		[[nodiscard]] constexpr type length()const noexcept { return std::sqrt(lengthsq()); }

		[[nodiscard]] constexpr type lengthsq()const noexcept { return x * x + y * y; }

		[[nodiscard]] constexpr type2 normalize()const noexcept { return *this / length(); }

		template<class T>
		[[nodiscard]] constexpr auto dot(const type2<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.x + y * _v.y; }

		template<class T>
		[[nodiscard]] constexpr auto cross(const type2<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.y - y * _v.x; }
	};

	template<class type>
	struct type3
	{
		type x, y, z;
		type3() : x(0), y(0), z(0) {}

		constexpr type3(type x, type y, type z)noexcept : x(x), y(y), z(z) {}

		constexpr type3(type _v)noexcept : x(_v), y(_v), z(_v) {}

		template<class T>
		constexpr type3(const type3<T>& _v)noexcept : x(static_cast<type>(_v.x)), y(static_cast<type>(_v.y)) {}

		[[nodiscard]] constexpr bool operator==(const type3& _v)const noexcept { return x == _v.x && y == _v.y && z == _v.z; }

		[[nodiscard]] constexpr bool operator!=(const type3& _v)const noexcept { return x != _v.x && y != _v.y && z != _v.z; }

		[[nodiscard]] constexpr type3 operator+()const noexcept { return *this; }

		[[nodiscard]] constexpr type3 operator-()const noexcept { return { -x, -y, -z }; }

		constexpr type3 operator+(const type3& _v)const noexcept { return { x + _v.x, y + _v.y, z + _v.z }; }

		constexpr type3 operator-(const type3& _v)const noexcept { return { x - _v.x, y - _v.y, z - _v.z }; }

		constexpr type3 operator*(const type3& _v)const noexcept { return { x * _v.x, y * _v.y, z * _v.z }; }

		constexpr type3 operator/(const type3& _v)const noexcept { return { x / _v.x, y / _v.y, z / _v.z }; }

		type3 operator=(const type3& _v)noexcept { x = _v.x; y = _v.y; z = _v.z; return *this; }

		type3 operator+=(const type3& _v)noexcept { x += _v.x; y += _v.y; z += _v.z; return *this; }

		type3 operator-=(const type3& _v)noexcept { x -= _v.x; y -= _v.y; z -= _v.z; return *this; }

		type3 operator*=(const type3& _v)noexcept { x *= _v.x; y *= _v.y; z *= _v.z; return *this; }

		type3 operator/=(const type3& _v)noexcept { x /= _v.x; y /= _v.y; z /= _v.z; return *this; }

		type3 operator=(const type& _v)noexcept { x = _v; y = _v; z = _v; return *this; }

		type3 operator+=(const type& _v)noexcept { x += _v; y += _v; z += _v; return *this; }

		type3 operator-=(const type& _v)noexcept { x -= _v; y -= _v; z -= _v; return *this; }

		type3 operator*=(const type& _v)noexcept { x *= _v; y *= _v; z *= _v; return *this; }

		type3 operator/=(const type& _v)noexcept { x /= _v; y /= _v; z /= _v; return *this; }

		[[nodiscard]] constexpr type length()const noexcept { return std::sqrt(lengthsq()); }

		[[nodiscard]] constexpr type lengthsq()const noexcept { return x * x + y * y + z * z; }

		[[nodiscard]] constexpr type3 normalize()const noexcept { return *this / length(); }

		template<class T>
		[[nodiscard]] constexpr auto dot(const type3<T>& _v)const noexcept->decltype(x* _v.x) { return x * _v.x + y * _v.y + z * _v.z; }

		template<class T>
		[[nodiscard]] constexpr auto cross(const type3<T>& _v)const noexcept->type3<decltype(x* _v.x)> { return{ y * _v.z - z * _v.y, z * _v.x - x * _v.z, x * _v.y - y * _v.x }; }
	};

	template<class type>
	struct type4
	{
		type x, y, z, w;
		type4() : x(0), y(0), z(0), w(0) {}

		constexpr type4(type x, type y, type z, type w)noexcept : x(x), y(y), z(z), w(w) {}

		constexpr type4(type _v)noexcept : x(_v), y(_v), z(_v), w(_v) {}

		template<class T>
		constexpr type4(const type4<T>& _v)noexcept : x(static_cast<type>(_v.x)), y(static_cast<type>(_v.y)) {}

		[[nodiscard]] constexpr bool operator==(const type4& _v)const noexcept { return x == _v.x && y == _v.y && z == _v.z && w == _v.w; }

		[[nodiscard]] constexpr bool operator!=(const type4& _v)const noexcept { return x != _v.x && y != _v.y && z != _v.z && w != _v.w; }

		[[nodiscard]] constexpr type4 operator+()const noexcept { return *this; }

		[[nodiscard]] constexpr type4 operator-()const noexcept { return { -x, -y, -z, -w }; }

		constexpr type4 operator+(const type4& _v)const noexcept { return { x + _v.x, y + _v.y, z + _v.z, w + _v.w }; }

		constexpr type4 operator-(const type4& _v)const noexcept { return { x - _v.x, y - _v.y, z - _v.z, w - _v.w }; }

		constexpr type4 operator*(const type4& _v)const noexcept { return { x * _v.x, y * _v.y, z * _v.z, w * _v.w }; }

		constexpr type4 operator/(const type4& _v)const noexcept { return { x / _v.x, y / _v.y, z / _v.z, w / _v.w }; }

		type4 operator=(const type4& _v)noexcept { x = _v.x; y = _v.y; z = _v.z; w = _v.w; return *this; }

		type4 operator+=(const type4& _v)noexcept { x += _v.x; y += _v.y; z += _v.z; w += _v.w; return *this; }

		type4 operator-=(const type4& _v)noexcept { x -= _v.x; y -= _v.y; z -= _v.z; w -= _v.w; return *this; }

		type4 operator*=(const type4& _v)noexcept { x *= _v.x; y *= _v.y; z *= _v.z; w *= _v.w; return *this; }

		type4 operator/=(const type4& _v)noexcept { x /= _v.x; y /= _v.y; z /= _v.z; w /= _v.w; return *this; }

		type4 operator=(const type& _v)noexcept { x = _v; y = _v; z = _v; w = _v; return *this; }

		type4 operator+=(const type& _v)noexcept { x += _v; y += _v; z += _v; w += _v; return *this; }

		type4 operator-=(const type& _v)noexcept { x -= _v; y -= _v; z -= _v; w -= _v; return *this; }

		type4 operator*=(const type& _v)noexcept { x *= _v; y *= _v; z *= _v; w *= _v; return *this; }

		type4 operator/=(const type& _v)noexcept { x /= _v; y /= _v; z /= _v; w /= _v; return *this; }
	};
}

namespace gl
{
	using int2 = detail::type2<int>;
	using int3 = detail::type3<int>;
	using int4 = detail::type4<int>;
	using uint = unsigned int;
	using uint2 = detail::type2<uint>;
	using uint3 = detail::type3<uint>;
	using uint4 = detail::type4<uint>;
	using float2 = detail::type2<float>;
	using float3 = detail::type3<float>;
	using float4 = detail::type4<float>;

	struct matrix
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

		constexpr matrix()noexcept :
			_11(0), _12(0), _13(0), _14(0),
			_21(0), _22(0), _23(0), _24(0),
			_31(0), _32(0), _33(0), _34(0),
			_41(0), _42(0), _43(0), _44(0) {}

		constexpr matrix(
			float _m11, float _m12, float _m13, float _m14,
			float _m21, float _m22, float _m23, float _m24,
			float _m31, float _m32, float _m33, float _m34,
			float _m41, float _m42, float _m43, float _m44)noexcept :
			_11(_m11), _12(_m12), _13(_m13), _14(_m14),
			_21(_m21), _22(_m22), _23(_m23), _24(_m24),
			_31(_m31), _32(_m32), _33(_m33), _34(_m34),
			_41(_m41), _42(_m42), _43(_m43), _44(_m44) {}

		constexpr matrix(const float4& _r1, const float4& _r2, const float4& _r3, const float4& _r4)noexcept :
			_11(_r1.x), _12(_r1.y), _13(_r1.z), _14(_r1.w),
			_21(_r2.x), _22(_r2.y), _23(_r2.z), _24(_r2.w),
			_31(_r3.x), _32(_r3.y), _33(_r3.z), _34(_r3.w),
			_41(_r4.x), _42(_r4.y), _43(_r4.z), _44(_r4.w) {}

		constexpr matrix(const matrix& _m)noexcept :
			_11(_m._11), _12(_m._12), _13(_m._13), _14(_m._14),
			_21(_m._21), _22(_m._22), _23(_m._23), _24(_m._24),
			_31(_m._31), _32(_m._32), _33(_m._33), _34(_m._34),
			_41(_m._41), _42(_m._42), _43(_m._43), _44(_m._44) {}

		matrix(const DirectX::XMMATRIX& _m)noexcept :
			_11(_m.r[0].m128_f32[0]), _12(_m.r[0].m128_f32[1]), _13(_m.r[0].m128_f32[2]), _14(_m.r[0].m128_f32[3]),
			_21(_m.r[1].m128_f32[0]), _22(_m.r[1].m128_f32[1]), _23(_m.r[1].m128_f32[2]), _24(_m.r[1].m128_f32[3]),
			_31(_m.r[2].m128_f32[0]), _32(_m.r[2].m128_f32[1]), _33(_m.r[2].m128_f32[2]), _34(_m.r[2].m128_f32[3]),
			_41(_m.r[3].m128_f32[0]), _42(_m.r[3].m128_f32[1]), _43(_m.r[3].m128_f32[2]), _44(_m.r[3].m128_f32[3]) {}

		operator DirectX::XMMATRIX()const noexcept { return { _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44 }; }

		constexpr bool operator==(const matrix& _m)const noexcept { return r[0] == _m.r[0] && r[1] == _m.r[1] && r[2] == _m.r[2] && r[3] == _m.r[3]; }

		constexpr bool operator!=(const matrix& _m)const noexcept { return r[0] != _m.r[0] || r[1] != _m.r[1] || r[2] != _m.r[2] || r[3] != _m.r[3]; }

		matrix operator*(const matrix& _m)const { return DirectX::XMMatrixMultiply(*this, _m); }

		matrix inverse()const { return DirectX::XMMatrixInverse(nullptr, *this); }

		matrix transpose()const { return DirectX::XMMatrixTranspose(*this); }

		static matrix scale(const float3& _scale) { return DirectX::XMMatrixScaling(_scale.x, _scale.y, _scale.z); }

		static matrix rotate(const float3& _radian) { return DirectX::XMMatrixRotationRollPitchYaw(_radian.x, _radian.y, _radian.z); }

		static matrix translation(const float3& _position) { return DirectX::XMMatrixTranslation(_position.x, _position.y, _position.z); }
	};

	inline float3 operator*(const float3& _v, const matrix& _m)
	{
		const DirectX::XMVECTOR value = DirectX::XMVector3TransformCoord({ _v.x, _v.y, _v.z, 1.0f }, _m);
		return { value.m128_f32[0], value.m128_f32[1], value.m128_f32[2] };
	}

	inline float4 operator*(const float4& _v, const matrix& _m)
	{
		const DirectX::XMVECTOR value = DirectX::XMVector4Transform({ _v.x, _v.y, _v.z, _v.w }, _m);
		return { value.m128_f32[0], value.m128_f32[1], value.m128_f32[2], value.m128_f32[3] };
	}
}

namespace gl::math
{
	static constexpr float PI = 3.14159265359f;

	template<class T>
	[[nodiscard]] inline constexpr T abs(T _value) { return (_value < static_cast<T>(0)) ? -_value : _value; }

	template<class T>
	[[nodiscard]] inline constexpr T radian(T _degree) { return _degree * (PI / 180.0f); }

	template<class T>
	[[nodiscard]] inline constexpr T degree(T _radian) { return _radian * (180.0f / PI); }

	template<class T>
	[[nodiscard]] inline constexpr T lerp(T _a, T _b, T _f) { return _a + _f * (_b - _a); }

	template<class T>
	[[nodiscard]] inline constexpr T clamp(T _v, T _low, T _high) { return (_low <= _v && _v <= _high) ? _v : (_v < _low) ? _low : _high; }
}