#ifndef _CLENCH_MATH_MAT_OP_INL_
#define _CLENCH_MATH_MAT_OP_INL_

#include "mat_def.inl"

template <typename T>
FORCEINLINE clench::math::Mat1x1<T> operator*(
	const clench::math::Mat1x1<T> &lhs,
	const clench::math::Mat1x1<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat1x1<T> operator*(
	const clench::math::Mat1x2<T> &lhs,
	const clench::math::Mat2x1<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat1x1<T> operator*(
	const clench::math::Mat1x3<T> &lhs,
	const clench::math::Mat3x1<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat1x1<T> operator*(
	const clench::math::Mat1x4<T> &lhs,
	const clench::math::Mat4x1<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat2x2<T> operator*(
	const clench::math::Mat2x1<T> &lhs,
	const clench::math::Mat1x2<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0],
		lhs[0][0] * rhs[0][1],

		lhs[1][0] * rhs[0][0],
		lhs[1][0] * rhs[0][1]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat2x2<T> operator*(
	const clench::math::Mat2x2<T> &lhs,
	const clench::math::Mat2x2<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat2x2<T> operator*(
	const clench::math::Mat2x3<T> &lhs,
	const clench::math::Mat3x2<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat2x2<T> operator*(
	const clench::math::Mat2x4<T> &lhs,
	const clench::math::Mat4x2<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1] + lhs[0][3] * rhs[3][1],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0] + lhs[1][3] * rhs[3][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1] + lhs[1][3] * rhs[3][1]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat3x3<T> operator*(
	const clench::math::Mat3x1<T> &lhs,
	const clench::math::Mat1x3<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0],
		lhs[0][0] * rhs[0][1],
		lhs[0][0] * rhs[0][2],

		lhs[1][0] * rhs[0][0],
		lhs[1][0] * rhs[0][1],
		lhs[1][0] * rhs[0][2],

		lhs[2][0] * rhs[0][0],
		lhs[2][0] * rhs[0][1],
		lhs[2][0] * rhs[0][2]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat3x3<T> operator*(
	const clench::math::Mat3x2<T> &lhs,
	const clench::math::Mat2x3<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1],
		lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1],
		lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2],

		lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0],
		lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1],
		lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat3x3<T> operator*(
	const clench::math::Mat3x3<T> &lhs,
	const clench::math::Mat3x3<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1],
		lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1],
		lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2],

		lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0],
		lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1],
		lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat3x3<T> operator*(
	const clench::math::Mat3x4<T> &lhs,
	const clench::math::Mat4x3<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1] + lhs[0][3] * rhs[3][1],
		lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2] + lhs[0][3] * rhs[3][2],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0] + lhs[1][3] * rhs[3][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1] + lhs[1][3] * rhs[3][1],
		lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2] + lhs[1][3] * rhs[3][2],

		lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0] + lhs[2][3] * rhs[3][0],
		lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1] + lhs[2][3] * rhs[3][1],
		lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2] + lhs[2][3] * rhs[3][2]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat4x4<T> operator*(
	const clench::math::Mat4x1<T> &lhs,
	const clench::math::Mat1x4<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0],
		lhs[0][0] * rhs[0][1],
		lhs[0][0] * rhs[0][2],
		lhs[0][0] * rhs[0][3],

		lhs[1][0] * rhs[0][0],
		lhs[1][0] * rhs[0][1],
		lhs[1][0] * rhs[0][2],
		lhs[1][0] * rhs[0][3],

		lhs[2][0] * rhs[0][0],
		lhs[2][0] * rhs[0][1],
		lhs[2][0] * rhs[0][2],
		lhs[2][0] * rhs[0][3],

		lhs[3][0] * rhs[0][0],
		lhs[3][0] * rhs[0][1],
		lhs[3][0] * rhs[0][2],
		lhs[3][0] * rhs[0][3]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat4x4<T> operator*(
	const clench::math::Mat4x2<T> &lhs,
	const clench::math::Mat2x4<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1],
		lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2],
		lhs[0][0] * rhs[0][3] + lhs[0][1] * rhs[1][3],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1],
		lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2],
		lhs[1][0] * rhs[0][3] + lhs[1][1] * rhs[1][3],

		lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0],
		lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1],
		lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2],
		lhs[2][0] * rhs[0][3] + lhs[2][1] * rhs[1][3],

		lhs[3][0] * rhs[0][0] + lhs[3][1] * rhs[1][0],
		lhs[3][0] * rhs[0][1] + lhs[3][1] * rhs[1][1],
		lhs[3][0] * rhs[0][2] + lhs[3][1] * rhs[1][2],
		lhs[3][0] * rhs[0][3] + lhs[3][1] * rhs[1][3]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat4x4<T> operator*(
	const clench::math::Mat4x3<T> &lhs,
	const clench::math::Mat3x4<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1],
		lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2],
		lhs[0][0] * rhs[0][3] + lhs[0][1] * rhs[1][3] + lhs[0][2] * rhs[2][3],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1],
		lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2],
		lhs[1][0] * rhs[0][3] + lhs[1][1] * rhs[1][3] + lhs[1][2] * rhs[2][3],

		lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0],
		lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1],
		lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2],
		lhs[2][0] * rhs[0][3] + lhs[2][1] * rhs[1][3] + lhs[2][2] * rhs[2][3],

		lhs[3][0] * rhs[0][0] + lhs[3][1] * rhs[1][0] + lhs[3][2] * rhs[2][0],
		lhs[3][0] * rhs[0][1] + lhs[3][1] * rhs[1][1] + lhs[3][2] * rhs[2][1],
		lhs[3][0] * rhs[0][2] + lhs[3][1] * rhs[1][2] + lhs[3][2] * rhs[2][2],
		lhs[3][0] * rhs[0][3] + lhs[3][1] * rhs[1][3] + lhs[3][2] * rhs[2][3]
	};
}

template <typename T>
FORCEINLINE clench::math::Mat4x4<T> operator*(
	const clench::math::Mat4x4<T> &lhs,
	const clench::math::Mat4x4<T> &rhs) {
	return {
		lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0],
		lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1] + lhs[0][3] * rhs[3][1],
		lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2] + lhs[0][3] * rhs[3][1],
		lhs[0][0] * rhs[0][3] + lhs[0][1] * rhs[1][3] + lhs[0][2] * rhs[2][3] + lhs[0][3] * rhs[3][1],

		lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0] + lhs[1][3] * rhs[3][0],
		lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1] + lhs[1][3] * rhs[3][1],
		lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2] + lhs[1][3] * rhs[3][1],
		lhs[1][0] * rhs[0][3] + lhs[1][1] * rhs[1][3] + lhs[1][2] * rhs[2][3] + lhs[1][3] * rhs[3][1],

		lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0] + lhs[2][3] * rhs[3][0],
		lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1] + lhs[2][3] * rhs[3][1],
		lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2] + lhs[2][3] * rhs[3][1],
		lhs[2][0] * rhs[0][3] + lhs[2][1] * rhs[1][3] + lhs[2][2] * rhs[2][3] + lhs[2][3] * rhs[3][1],

		lhs[3][0] * rhs[0][0] + lhs[3][1] * rhs[1][0] + lhs[3][2] * rhs[2][0] + lhs[3][3] * rhs[3][0],
		lhs[3][0] * rhs[0][1] + lhs[3][1] * rhs[1][1] + lhs[3][2] * rhs[2][1] + lhs[3][3] * rhs[3][1],
		lhs[3][0] * rhs[0][2] + lhs[3][1] * rhs[1][2] + lhs[3][2] * rhs[2][2] + lhs[3][3] * rhs[3][1],
		lhs[3][0] * rhs[0][3] + lhs[3][1] * rhs[1][3] + lhs[3][2] * rhs[2][3] + lhs[3][3] * rhs[3][1]
	};
}

#endif
