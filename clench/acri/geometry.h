#ifndef _CLENCH_ACRI_GEOMETRY_H_
#define _CLENCH_ACRI_GEOMETRY_H_

#include "basedefs.h"
#include <clench/ghal.h>
#include <clench/math/vec.h>

namespace clench {
	namespace acri {
		enum class GeometryKind {
			Triangle = 0,
			Polygon,
			Oval
			Path
		};

		class Geometry {
		public:
			GeometryKind geometryKind;

			CLCACRI_API Geometry(GeometryKind geometryKind);
			CLCACRI_API virtual ~Geometry();
		};

		struct TriangleVertex {
			math::Vec3f position;
			math::Vec4f color;
		};

		class TriangleGeometry {
		public:
			TriangleVertex vertices[3];

			CLCACRI_API TriangleGeometry();
			CLCACRI_API virtual ~TriangleGeometry();
		};

		class PolygonGeometry {
		public:
			CLCACRI_API PolygonGeometry();
			CLCACRI_API virtual ~PolygonGeometry();
		};

		class OvalGeometry {
		public:
			float direction;
			math::Vec2f scale;

			CLCACRI_API OvalGeometry();
			CLCACRI_API virtual ~OvalGeometry();
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

		class PathGeometry {
		public:
			float initX, initY;
			std::vector<PathInstruction> instructions;

			CLCACRI_API PathGeometry();
			CLCACRI_API virtual ~PathGeometry();
		};
	}
}

#endif
