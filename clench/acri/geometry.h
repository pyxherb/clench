#ifndef _CLENCH_ACRI_GEOMETRY_H_
#define _CLENCH_ACRI_GEOMETRY_H_

#include "resource.h"
#include <clench/ghal.h>
#include <clench/math/vec.h>
#include <peff/containers/dynarray.h>

namespace clench {
	namespace acri {
		struct TriangleParams {
			math::Vec2f vertices[3];
		};

		struct RectParams {
			float left;
			float right;
			float top;
			float bottom;
		};

		struct EllipseParams {
			float direction;
			math::Vec2f scale;
		};

		enum class PathInstructionOpcode {
			Line,
			QuadraticBezierCurve,
			SmoothQuadraticBezierCurve,
			BezierCurve,
			SmoothBezierCurve,
			Close
		};

		struct LinePathInstructionParams {
			float x, y;
		};

		struct QuadraticBezierCurvePathInstructionParams {
		};

		struct BezierCurvePathInstructionParams {
		};

		struct PathInstruction {
			union {
				LinePathInstructionParams asLine;
				QuadraticBezierCurvePathInstructionParams asQuadraticBezierCurve;
				BezierCurvePathInstructionParams asBezierCurve;
			} params;
			PathInstructionOpcode opcode;
		};

		struct PathParams {
			float initX, initY;
			PathInstruction *instructions;
			size_t nInstructions;
		};
	}
}

#endif
