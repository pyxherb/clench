#include "geometry.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API Geometry::Geometry(ACRIDevice *device, GeometryKind geometryKind) : ACRIResource(device), geometryKind(geometryKind) {
}

CLCACRI_API Geometry::~Geometry() {
}

CLCACRI_API TriangleGeometry::TriangleGeometry(ACRIDevice *device) : Geometry(device, GeometryKind::Triangle) {
}

CLCACRI_API TriangleGeometry::~TriangleGeometry() {
}

CLCACRI_API EllipseGeometry::EllipseGeometry(ACRIDevice *device) : Geometry(device, GeometryKind::Ellipse) {
}
CLCACRI_API EllipseGeometry::~EllipseGeometry() {
}

CLCACRI_API PathGeometry::PathGeometry(ACRIDevice *device) : Geometry(device, GeometryKind::Path) {
}
CLCACRI_API PathGeometry::~PathGeometry() {
}
