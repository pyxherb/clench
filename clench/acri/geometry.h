#ifndef _CLENCH_ACRI_GEOMETRY_H_
#define _CLENCH_ACRI_GEOMETRY_H_

#include "resource.h"
#include <clench/ghal.h>
#include <clench/math/vec.h>
#include <peff/containers/dynarray.h>

namespace clench {
	namespace acri {
		enum class GeometryKind {
			Triangle = 0,
			Ellipse,
			Path
		};

		class Geometry : public ACRIResource {
		public:
			GeometryKind geometryKind;

			CLENCH_NO_COPY_MOVE_METHODS(Geometry);

			CLCACRI_API Geometry(ACRIDevice *device, GeometryKind geometryKind);
			CLCACRI_API virtual ~Geometry();
		};

		class TriangleGeometry : public Geometry {
		public:
			math::Vec3f vertices[3];

			CLENCH_NO_COPY_MOVE_METHODS(TriangleGeometry);

			CLCACRI_API TriangleGeometry(ACRIDevice *device);
			CLCACRI_API virtual ~TriangleGeometry();
		};

		class EllipseGeometry : public Geometry {
		public:
			float direction;
			math::Vec2f scale;

			CLENCH_NO_COPY_MOVE_METHODS(EllipseGeometry);

			CLCACRI_API EllipseGeometry(ACRIDevice *device);
			CLCACRI_API virtual ~EllipseGeometry();
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

		class PathGeometry : public Geometry {
		public:
			float initX, initY;
			peff::DynArray<PathInstruction> instructions;

			CLENCH_NO_COPY_MOVE_METHODS(PathGeometry);

			CLCACRI_API PathGeometry(ACRIDevice *device);
			CLCACRI_API virtual ~PathGeometry();
		};
	}
}

#endif
