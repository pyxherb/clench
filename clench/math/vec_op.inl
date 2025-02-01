#ifndef _CLENCH_MATH_VEC_OP_INL_
#define _CLENCH_MATH_VEC_OP_INL_

#include "vec_def.inl"

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> &operator+=(clench::math::Vec1<T> &lhs, T rhs) {
	lhs.x += rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> operator+(const clench::math::Vec1<T> &lhs, T rhs) {
	clench::math::Vec1<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> &operator-=(clench::math::Vec1<T> &lhs, T rhs) {
	lhs.x -= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> operator-(const clench::math::Vec1<T> &lhs, T rhs) {
	clench::math::Vec1<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> &operator*=(clench::math::Vec1<T> &lhs, T rhs) {
	lhs.x *= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> operator*(const clench::math::Vec1<T> &lhs, T rhs) {
	clench::math::Vec1<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> &operator/=(clench::math::Vec1<T> &lhs, T rhs) {
	lhs.x /= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> operator/(const clench::math::Vec1<T> &lhs, T rhs) {
	clench::math::Vec1<T> result = lhs;
	result /= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> &operator+=(clench::math::Vec2<T> &lhs, T rhs) {
	lhs.x += rhs;
	lhs.y += rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> operator+(const clench::math::Vec2<T> &lhs, T rhs) {
	clench::math::Vec2<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> &operator-=(clench::math::Vec2<T> &lhs, T rhs) {
	lhs.x -= rhs;
	lhs.y -= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> operator-(const clench::math::Vec2<T> &lhs, T rhs) {
	clench::math::Vec2<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> &operator*=(clench::math::Vec2<T> &lhs, T rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> operator*(const clench::math::Vec2<T> &lhs, T rhs) {
	clench::math::Vec2<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> &operator/=(clench::math::Vec2<T> &lhs, T rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> operator/(const clench::math::Vec2<T> &lhs, T rhs) {
	clench::math::Vec2<T> result = lhs;
	result /= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> &operator+=(clench::math::Vec3<T> &lhs, T rhs) {
	lhs.x += rhs;
	lhs.y += rhs;
	lhs.z += rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> operator+(const clench::math::Vec3<T> &lhs, T rhs) {
	clench::math::Vec3<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> &operator-=(clench::math::Vec3<T> &lhs, T rhs) {
	lhs.x -= rhs;
	lhs.y -= rhs;
	lhs.z -= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> operator-(const clench::math::Vec3<T> &lhs, T rhs) {
	clench::math::Vec3<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> &operator*=(clench::math::Vec3<T> &lhs, T rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> operator*(const clench::math::Vec3<T> &lhs, T rhs) {
	clench::math::Vec3<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> &operator/=(clench::math::Vec3<T> &lhs, T rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> operator/(const clench::math::Vec3<T> &lhs, T rhs) {
	clench::math::Vec3<T> result = lhs;
	result /= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> &operator+=(clench::math::Vec4<T> &lhs, T rhs) {
	lhs.x += rhs;
	lhs.y += rhs;
	lhs.z += rhs;
	lhs.w += rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> operator+(const clench::math::Vec4<T> &lhs, T rhs) {
	clench::math::Vec4<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> &operator-=(clench::math::Vec4<T> &lhs, T rhs) {
	lhs.x -= rhs;
	lhs.y -= rhs;
	lhs.z -= rhs;
	lhs.w -= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> operator-(const clench::math::Vec4<T> &lhs, T rhs) {
	clench::math::Vec4<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> &operator*=(clench::math::Vec4<T> &lhs, T rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	lhs.w *= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> operator*(const clench::math::Vec4<T> &lhs, T rhs) {
	clench::math::Vec4<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> &operator/=(clench::math::Vec4<T> &lhs, T rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	lhs.w /= rhs;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> operator/(const clench::math::Vec4<T> &lhs, T rhs) {
	clench::math::Vec4<T> result = lhs;
	result /= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> &operator+=(clench::math::Vec1<T> &lhs, const clench::math::Vec1<T> &rhs) {
	lhs.x += rhs.x;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> operator+(const clench::math::Vec1<T> &lhs, const clench::math::Vec1<T> &rhs) {
	clench::math::Vec2<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> &operator-=(clench::math::Vec1<T> &lhs, const clench::math::Vec1<T> &rhs) {
	lhs.x -= rhs.x;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> operator-(const clench::math::Vec1<T> &lhs, const clench::math::Vec1<T> &rhs) {
	clench::math::Vec1<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> &operator*=(clench::math::Vec1<T> &lhs, const clench::math::Vec1<T> &rhs) {
	lhs.x *= rhs.x;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> operator*(const clench::math::Vec1<T> &lhs, const clench::math::Vec1<T> &rhs) {
	clench::math::Vec1<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> &operator/=(clench::math::Vec1<T> &lhs, const clench::math::Vec1<T> &rhs) {
	lhs.x /= rhs.x;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec1<T> operator/(const clench::math::Vec1<T> &lhs, const clench::math::Vec1<T> &rhs) {
	clench::math::Vec1<T> result = lhs;
	result /= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> &operator+=(clench::math::Vec2<T> &lhs, const clench::math::Vec2<T> &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> operator+(const clench::math::Vec2<T> &lhs, const clench::math::Vec2<T> &rhs) {
	clench::math::Vec2<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> &operator-=(clench::math::Vec2<T> &lhs, const clench::math::Vec2<T> &rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> operator-(const clench::math::Vec2<T> &lhs, const clench::math::Vec2<T> &rhs) {
	clench::math::Vec2<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> &operator*=(clench::math::Vec2<T> &lhs, const clench::math::Vec2<T> &rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> operator*(const clench::math::Vec2<T> &lhs, const clench::math::Vec2<T> &rhs) {
	clench::math::Vec2<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> &operator/=(clench::math::Vec2<T> &lhs, const clench::math::Vec2<T> &rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec2<T> operator/(const clench::math::Vec2<T> &lhs, const clench::math::Vec2<T> &rhs) {
	clench::math::Vec2<T> result = lhs;
	result /= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> &operator+=(clench::math::Vec3<T> &lhs, const clench::math::Vec3<T> &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> operator+(const clench::math::Vec3<T> &lhs, const clench::math::Vec3<T> &rhs) {
	clench::math::Vec3<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> &operator-=(clench::math::Vec3<T> &lhs, const clench::math::Vec3<T> &rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> operator-(const clench::math::Vec3<T> &lhs, const clench::math::Vec3<T> &rhs) {
	clench::math::Vec3<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> &operator*=(clench::math::Vec3<T> &lhs, const clench::math::Vec3<T> &rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> operator*(const clench::math::Vec3<T> &lhs, const clench::math::Vec3<T> &rhs) {
	clench::math::Vec3<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> &operator/=(clench::math::Vec3<T> &lhs, const clench::math::Vec3<T> &rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec3<T> operator/(const clench::math::Vec3<T> &lhs, const clench::math::Vec3<T> &rhs) {
	clench::math::Vec3<T> result = lhs;
	result /= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> &operator+=(clench::math::Vec4<T> &lhs, const clench::math::Vec4<T> &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> operator+(const clench::math::Vec4<T> &lhs, const clench::math::Vec4<T> &rhs) {
	clench::math::Vec4<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> &operator-=(clench::math::Vec4<T> &lhs, const clench::math::Vec4<T> &rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	lhs.w -= rhs.w;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> operator-(const clench::math::Vec4<T> &lhs, const clench::math::Vec4<T> &rhs) {
	clench::math::Vec4<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> &operator*=(clench::math::Vec4<T> &lhs, const clench::math::Vec4<T> &rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;
	lhs.w *= rhs.w;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> operator*(const clench::math::Vec4<T> &lhs, const clench::math::Vec4<T> &rhs) {
	clench::math::Vec4<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> &operator/=(clench::math::Vec4<T> &lhs, const clench::math::Vec4<T> &rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;
	lhs.w /= rhs.w;
	return lhs;
}

template <typename T>
CLENCH_FORCEINLINE clench::math::Vec4<T> operator/(const clench::math::Vec4<T> &lhs, const clench::math::Vec4<T> &rhs) {
	clench::math::Vec4<T> result = lhs;
	result /= rhs;
	return result;
}

#if CLCMATH_X86_INTRINSICS
template <>
CLENCH_FORCEINLINE clench::math::Vec2f &operator+=(clench::math::Vec2f &lhs, const clench::math::Vec2f &rhs) {
	#if CLCMATH_SSE_INTRINSICS
	lhs.m128 = _mm_add_ps(lhs.m128, rhs.m128);
	#endif

	return lhs;
}

template <>
CLENCH_FORCEINLINE clench::math::Vec2f operator+(const clench::math::Vec2f &lhs, const clench::math::Vec2f &rhs) {
	return { _mm_add_ps(lhs.m128, rhs.m128) };
}

template <>
CLENCH_FORCEINLINE clench::math::Vec3f &operator+=(clench::math::Vec3f &lhs, const clench::math::Vec3f &rhs) {
	#if CLCMATH_SSE_INTRINSICS
	lhs.m128 = _mm_add_ps(lhs.m128, rhs.m128);
	#endif

	return lhs;
}

template <>
CLENCH_FORCEINLINE clench::math::Vec3f operator+(const clench::math::Vec3f &lhs, const clench::math::Vec3f &rhs) {
	return { _mm_add_ps(lhs.m128, rhs.m128) };
}

template <>
CLENCH_FORCEINLINE clench::math::Vec4f &operator+=(clench::math::Vec4f &lhs, const clench::math::Vec4f &rhs) {
	#if CLCMATH_SSE_INTRINSICS
	lhs.m128 = _mm_add_ps(lhs.m128, rhs.m128);
	#endif

	return lhs;
}

template <>
CLENCH_FORCEINLINE clench::math::Vec4f operator+(const clench::math::Vec4f &lhs, const clench::math::Vec4f &rhs) {
	return { _mm_add_ps(lhs.m128, rhs.m128) };
}

template <>
CLENCH_FORCEINLINE clench::math::Vec2d &operator+=(clench::math::Vec2d &lhs, const clench::math::Vec2d &rhs) {
	#if CLCMATH_SSE_INTRINSICS
	lhs.m128d = _mm_add_pd(lhs.m128d, rhs.m128d);
	#endif

	return lhs;
}

template <>
CLENCH_FORCEINLINE clench::math::Vec2d operator+(const clench::math::Vec2d &lhs, const clench::math::Vec2d &rhs) {
	return { _mm_add_pd(lhs.m128d, rhs.m128d) };
}

template <>
CLENCH_FORCEINLINE clench::math::Vec3d &operator+=(clench::math::Vec3d &lhs, const clench::math::Vec3d &rhs) {
	#if CLCMATH_SSE_INTRINSICS
	lhs.m128d.m128dLow = _mm_add_pd(lhs.m128d.m128dLow, rhs.m128d.m128dLow);
	lhs.m128d.m128dHigh = _mm_add_pd(lhs.m128d.m128dHigh, rhs.m128d.m128dHigh);
	#endif

	return lhs;
}

template <>
CLENCH_FORCEINLINE clench::math::Vec3d operator+(const clench::math::Vec3d &lhs, const clench::math::Vec3d &rhs) {
	return { _mm_add_pd(lhs.m128d.m128dLow, rhs.m128d.m128dLow), _mm_add_pd(lhs.m128d.m128dHigh, rhs.m128d.m128dHigh) };
}

template <>
CLENCH_FORCEINLINE clench::math::Vec4d &operator+=(clench::math::Vec4d &lhs, const clench::math::Vec4d &rhs) {
	#if CLCMATH_SSE_INTRINSICS
	lhs.m128d.m128dLow = _mm_add_pd(lhs.m128d.m128dLow, rhs.m128d.m128dLow);
	lhs.m128d.m128dHigh = _mm_add_pd(lhs.m128d.m128dHigh, rhs.m128d.m128dHigh);
	#endif

	return lhs;
}

template <>
CLENCH_FORCEINLINE clench::math::Vec4d operator+(const clench::math::Vec4d &lhs, const clench::math::Vec4d &rhs) {
	return { _mm_add_pd(lhs.m128d.m128dLow, rhs.m128d.m128dLow), _mm_add_pd(lhs.m128d.m128dHigh, rhs.m128d.m128dHigh) };
}
#endif

namespace clench {
	namespace math {
		template<typename T>
		CLENCH_FORCEINLINE T dot(const Vec1<T> &lhs, const Vec1<T> &rhs) {
			return lhs.x * rhs.x;
		}

		template<typename T>
		CLENCH_FORCEINLINE T dot(const Vec2<T> &lhs, const Vec2<T> &rhs) {
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}

		template<typename T>
		CLENCH_FORCEINLINE T dot(const Vec3<T> &lhs, const Vec3<T> &rhs) {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}

		template<typename T>
		CLENCH_FORCEINLINE T dot(const Vec4<T> &lhs, const Vec4<T> &rhs) {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
		}

		template<typename T>
		CLENCH_FORCEINLINE T crossZ(const Vec2<T> &lhs, const Vec2<T> &rhs) {
			return lhs.x * rhs.y - rhs.x * lhs.y;
		}

		template<typename T>
		CLENCH_FORCEINLINE Vec3<T> cross(const Vec3<T> &lhs, const Vec3<T> &rhs) {
			return {
				lhs.y * rhs.z - lhs.z * rhs.y,
				lhs.z * rhs.x - lhs.x * rhs.z,
				lhs.x * rhs.y - lhs.y * rhs.x
			};
		}
	}
}

#endif
