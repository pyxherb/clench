#ifndef _CLENCH_MATH_VEC_DEF_INL_
#define _CLENCH_MATH_VEC_DEF_INL_

#include "intrinsics.h"
#include <clench/utils/assert.h>
#include <cstring>

namespace clench {
	namespace math {
		template <typename T>
		struct Vec1 {
			union {
				T data[1];
				struct {
					T x;
				};
			};

			FORCEINLINE T &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 1);
				return data[index];
			}
			FORCEINLINE const T &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 1);
				return data[index];
			}
		};

		template <typename T>
		struct Vec2 {
			union {
				T data[2];
				struct {
					T x;
					T y;
				};
			};

			FORCEINLINE T &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 2);
				return data[index];
			}
			FORCEINLINE const T &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 2);
				return data[index];
			}
		};

		template <typename T>
		struct Vec3 {
			union {
				T data[3];
				struct {
					T x;
					T y;
					T z;
				};
			};

			FORCEINLINE T &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 3);
				return data[index];
			}
			FORCEINLINE const T &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 3);
				return data[index];
			}
		};

		template <typename T>
		struct Vec4 {
			union {
				T data[4];
				struct {
					T x;
					T y;
					T z;
					T w;
				};
			};

			FORCEINLINE T &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 4);
				return data[index];
			}
			FORCEINLINE const T &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 4);
				return data[index];
			}
		};

#if CLCMATH_X86_INTRINSICS
		template <>
		struct alignas(16) Vec2<float> {
			union {
				float data[2];
				struct {
					float x;
					float y;
				};
	#if defined(CLCMATH_SSE_INTRINSICS)
				__m128 m128;
	#endif
			};

			FORCEINLINE Vec2(float x, float y) : x(x), y(y) {}
			FORCEINLINE Vec2(__m128 m128) : m128(m128) {}

			FORCEINLINE float &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 2);
				return data[index];
			}
			FORCEINLINE const float &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 2);
				return data[index];
			}
		};

		template <>
		struct alignas(16) Vec3<float> {
			union {
				float data[3];
				struct {
					float x;
					float y;
					float z;
				};
	#if defined(CLCMATH_SSE_INTRINSICS)
				__m128 m128;
	#endif
			};

			FORCEINLINE Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
			FORCEINLINE Vec3(__m128 m128) : m128(m128) {}

			FORCEINLINE float &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 3);
				return data[index];
			}
			FORCEINLINE const float &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 3);
				return data[index];
			}
		};

		template <>
		struct alignas(16) Vec4<float> {
			union {
				float data[4];
				struct {
					float x;
					float y;
					float z;
					float w;
				};
	#if defined(CLCMATH_SSE_INTRINSICS)
				__m128 m128;
	#endif
			};

			FORCEINLINE Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
			FORCEINLINE Vec4(__m128 m128) : m128(m128) {}

			FORCEINLINE float &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 4);
				return data[index];
			}
			FORCEINLINE const float &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 4);
				return data[index];
			}
		};

		template <>
		struct alignas(32) Vec3<double> {
			union {
				double data[3];
				struct {
					double x;
					double y;
					double z;
				};
	#if defined(CLCMATH_SSE_INTRINSICS)
				struct {
					__m128d m128dLow;
					__m128d m128dHigh;
				} m128d;
	#endif
	#if defined(CLCMATH_AVX_INTRINSICS)
				__m256 m256;
	#endif
			};

			FORCEINLINE Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
			FORCEINLINE Vec3(__m128d m128dLow, __m128d m128dHigh) : m128d({ m128dLow, m128dHigh }) {}

			FORCEINLINE double &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 3);
				return data[index];
			}
			FORCEINLINE const double &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 3);
				return data[index];
			}
		};

		template <>
		struct alignas(32) Vec4<double> {
			union {
				double data[4];
				struct {
					double x;
					double y;
					double z;
					double w;
				};
	#if defined(CLCMATH_SSE_INTRINSICS)
				struct {
					__m128d m128dLow;
					__m128d m128dHigh;
				} m128d;
	#endif
	#if defined(CLCMATH_AVX_INTRINSICS)
	#endif
			};

			FORCEINLINE Vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
			FORCEINLINE Vec4(__m128d m128dLow, __m128d m128dHigh) : m128d({ m128dLow, m128dHigh }) {}

			FORCEINLINE double &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 4);
				return data[index];
			}
			FORCEINLINE const double &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 4);
				return data[index];
			}
		};
#endif

		using Vec1f = Vec1<float>;
		using Vec1d = Vec1<double>;
		using Vec1i = Vec1<int>;
		using Vec1u = Vec1<unsigned int>;
		using Vec1b = Vec1<bool>;

		using Vec2f = Vec2<float>;
		using Vec2d = Vec2<double>;
		using Vec2i = Vec2<int>;
		using Vec2u = Vec2<unsigned int>;
		using Vec2b = Vec2<bool>;

		using Vec3f = Vec3<float>;
		using Vec3d = Vec3<double>;
		using Vec3i = Vec3<int>;
		using Vec3u = Vec3<unsigned int>;
		using Vec3b = Vec3<bool>;

		using Vec4f = Vec4<float>;
		using Vec4d = Vec4<double>;
		using Vec4i = Vec4<int>;
		using Vec4u = Vec4<unsigned int>;
		using Vec4b = Vec4<bool>;
	}
}

#endif
