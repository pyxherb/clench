#ifndef _CLENCH_ACRI_GEOMETRY_H_
#define _CLENCH_ACRI_GEOMETRY_H_

#include "resource.h"
#include <clench/ghal.h>
#include <clench/math/vec.h>

namespace clench {
	namespace acri {
		enum class GeometryKind {
			Triangle = 0,
			Polygon,
			Ellipse,
			Path
		};

		class Geometry : public ACRIResource {
		public:
			GeometryKind geometryKind;

			CLENCH_NO_COPY_MOVE_METHODS(Geometry);

			CLCACRI_API Geometry(GeometryKind geometryKind);
			CLCACRI_API virtual ~Geometry();
		};

		struct TriangleVertex {
			math::Vec3f position;
			math::Vec4f color;
		};

		class TriangleGeometry : public Geometry {
		public:
			TriangleVertex vertices[3];

			CLENCH_NO_COPY_MOVE_METHODS(TriangleGeometry);

			CLCACRI_API TriangleGeometry();
			CLCACRI_API virtual ~TriangleGeometry();

			CLCACRI_API virtual void dealloc() override;
		};

		class PolygonGeometry : public Geometry {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(PolygonGeometry);

			CLCACRI_API PolygonGeometry();
			CLCACRI_API virtual ~PolygonGeometry();

			CLCACRI_API virtual void dealloc() override;
		};

		class EllipseGeometry : public Geometry {
		public:
			float direction;
			math::Vec2f scale;

			CLENCH_NO_COPY_MOVE_METHODS(EllipseGeometry);

			CLCACRI_API EllipseGeometry();
			CLCACRI_API virtual ~EllipseGeometry();

			CLCACRI_API virtual void dealloc() override;
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
			std::vector<PathInstruction> instructions;

			CLENCH_NO_COPY_MOVE_METHODS(PathGeometry);

			CLCACRI_API PathGeometry();
			CLCACRI_API virtual ~PathGeometry();

			CLCACRI_API virtual void dealloc() override;
		};
	}
}

#endif
