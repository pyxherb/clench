#ifndef _CLENCH_MATH_MAT_DEF_INL_
#define _CLENCH_MATH_MAT_DEF_INL_

#include "vec.h"

namespace clench {
	namespace math {
		template <typename T>
		struct Mat1x1 {
			union {
				T data[1][1];
				Vec1<T> rows[1];
			};

			CLENCH_FORCEINLINE Vec1<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 1);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec1<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 1);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat1x2 {
			union {
				T data[1][2];
				Vec2<T> rows[1];
			};

			CLENCH_FORCEINLINE Vec2<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 1);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec2<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 1);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat1x3 {
			union {
				T data[1][3];
				Vec3<T> rows[1];
			};

			CLENCH_FORCEINLINE Vec3<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 1);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec3<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 1);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat1x4 {
			union {
				T data[1][4];
				Vec4<T> rows[1];
			};

			CLENCH_FORCEINLINE Vec4<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 1);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec4<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 1);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat2x1 {
			union {
				T data[2][1];
				Vec1<T> rows[2];
			};

			CLENCH_FORCEINLINE Vec1<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 2);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec1<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 2);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat2x2 {
			union {
				T data[2][2];
				Vec2<T> rows[2];
			};

			CLENCH_FORCEINLINE Vec2<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 2);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec2<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 2);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat2x3 {
			union {
				T data[2][3];
				Vec3<T> rows[2];
			};

			CLENCH_FORCEINLINE Vec3<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 2);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec3<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 2);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat2x4 {
			union {
				T data[2][4];
				Vec4<T> rows[2];
			};

			CLENCH_FORCEINLINE Vec4<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 2);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec4<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 2);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat3x1 {
			union {
				T data[3][1];
				Vec1<T> rows[3];
			};

			CLENCH_FORCEINLINE Vec2<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 3);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec2<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 3);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat3x2 {
			union {
				T data[3][2];
				Vec2<T> rows[3];
			};

			CLENCH_FORCEINLINE Vec2<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 3);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec2<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 3);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat3x3 {
			union {
				T data[3][3];
				Vec3<T> rows[3];
			};

			CLENCH_FORCEINLINE Vec3<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 3);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec3<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 3);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat3x4 {
			union {
				T data[3][4];
				Vec4<T> rows[3];
			};

			CLENCH_FORCEINLINE Vec4<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 3);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec4<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 3);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat4x1 {
			union {
				T data[4][1];
				Vec1<T> rows[4];
			};

			CLENCH_FORCEINLINE Vec1<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 4);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec1<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 4);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat4x2 {
			union {
				T data[4][2];
				Vec2<T> rows[4];
			};

			CLENCH_FORCEINLINE Vec2<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 4);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec2<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 4);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat4x3 {
			union {
				T data[4][3];
				Vec3<T> rows[4];
			};

			CLENCH_FORCEINLINE Vec3<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 4);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec3<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 4);
				return rows[index];
			}
		};

		template <typename T>
		struct Mat4x4 {
			union {
				T data[4][4];
				Vec4<T> rows[4];
			};

			CLENCH_FORCEINLINE Vec4<T> &operator[](size_t index) {
				CLCMATH_INDEX_ASSERT(index, 4);
				return rows[index];
			}
			CLENCH_FORCEINLINE const Vec4<T> &operator[](size_t index) const {
				CLCMATH_INDEX_ASSERT(index, 4);
				return rows[index];
			}
		};

		using Mat1x1f = Mat1x1<float>;
		using Mat1x1d = Mat1x1<double>;
		using Mat1x1i = Mat1x1<int>;
		using Mat1x1u = Mat1x1<unsigned int>;
		using Mat1x1b = Mat1x1<bool>;

		using Mat1x2f = Mat1x2<float>;
		using Mat1x2d = Mat1x2<double>;
		using Mat1x2i = Mat1x2<int>;
		using Mat1x2u = Mat1x2<unsigned int>;
		using Mat1x2b = Mat1x2<bool>;

		using Mat1x3f = Mat1x3<float>;
		using Mat1x3d = Mat1x3<double>;
		using Mat1x3i = Mat1x3<int>;
		using Mat1x3u = Mat1x3<unsigned int>;
		using Mat1x3b = Mat1x3<bool>;

		using Mat1x4f = Mat1x4<float>;
		using Mat1x4d = Mat1x4<double>;
		using Mat1x4i = Mat1x4<int>;
		using Mat1x4u = Mat1x4<unsigned int>;
		using Mat1x4b = Mat1x4<bool>;

		using Mat2x1f = Mat2x1<float>;
		using Mat2x1d = Mat2x1<double>;
		using Mat2x1i = Mat2x1<int>;
		using Mat2x1u = Mat2x1<unsigned int>;
		using Mat2x1b = Mat2x1<bool>;

		using Mat2x2f = Mat2x2<float>;
		using Mat2x2d = Mat2x2<double>;
		using Mat2x2i = Mat2x2<int>;
		using Mat2x2u = Mat2x2<unsigned int>;
		using Mat2x2b = Mat2x2<bool>;

		using Mat2x3f = Mat2x3<float>;
		using Mat2x3d = Mat2x3<double>;
		using Mat2x3i = Mat2x3<int>;
		using Mat2x3u = Mat2x3<unsigned int>;
		using Mat2x3b = Mat2x3<bool>;

		using Mat2x4f = Mat2x4<float>;
		using Mat2x4d = Mat2x4<double>;
		using Mat2x4i = Mat2x4<int>;
		using Mat2x4u = Mat2x4<unsigned int>;
		using Mat2x4b = Mat2x4<bool>;

		using Mat3x1f = Mat3x1<float>;
		using Mat3x1d = Mat3x1<double>;
		using Mat3x1i = Mat3x1<int>;
		using Mat3x1u = Mat3x1<unsigned int>;
		using Mat3x1b = Mat3x1<bool>;

		using Mat3x2f = Mat3x2<float>;
		using Mat3x2d = Mat3x2<double>;
		using Mat3x2i = Mat3x2<int>;
		using Mat3x2u = Mat3x2<unsigned int>;
		using Mat3x2b = Mat3x2<bool>;

		using Mat3x3f = Mat3x3<float>;
		using Mat3x3d = Mat3x3<double>;
		using Mat3x3i = Mat3x3<int>;
		using Mat3x3u = Mat3x3<unsigned int>;
		using Mat3x3b = Mat3x3<bool>;

		using Mat3x4f = Mat3x4<float>;
		using Mat3x4d = Mat3x4<double>;
		using Mat3x4i = Mat3x4<int>;
		using Mat3x4u = Mat3x4<unsigned int>;
		using Mat3x4b = Mat3x4<bool>;

		using Mat4x1f = Mat4x1<float>;
		using Mat4x1d = Mat4x1<double>;
		using Mat4x1i = Mat4x1<int>;
		using Mat4x1u = Mat4x1<unsigned int>;
		using Mat4x1b = Mat4x1<bool>;

		using Mat4x2f = Mat4x2<float>;
		using Mat4x2d = Mat4x2<double>;
		using Mat4x2i = Mat4x2<int>;
		using Mat4x2u = Mat4x2<unsigned int>;
		using Mat4x2b = Mat4x2<bool>;

		using Mat4x3f = Mat4x3<float>;
		using Mat4x3d = Mat4x3<double>;
		using Mat4x3i = Mat4x3<int>;
		using Mat4x3u = Mat4x3<unsigned int>;
		using Mat4x3b = Mat4x3<bool>;

		using Mat4x4f = Mat4x4<float>;
		using Mat4x4d = Mat4x4<double>;
		using Mat4x4i = Mat4x4<int>;
		using Mat4x4u = Mat4x4<unsigned int>;
		using Mat4x4b = Mat4x4<bool>;
	}
}

#endif
