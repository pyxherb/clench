#include "geometry.h"
#include "device.h"

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

CLCACRI_API void TriangleGeometry::dealloc() {
	peff::destroyAndRelease<TriangleGeometry>(device->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API EllipseGeometry::EllipseGeometry(ACRIDevice *device) : Geometry(device, GeometryKind::Ellipse) {
}
CLCACRI_API EllipseGeometry::~EllipseGeometry() {
}

CLCACRI_API void EllipseGeometry::dealloc() {
	peff::destroyAndRelease<EllipseGeometry>(device->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API PathGeometry::PathGeometry(ACRIDevice *device) : Geometry(device, GeometryKind::Path) {
}
CLCACRI_API PathGeometry::~PathGeometry() {
}

CLCACRI_API void PathGeometry::dealloc() {
	peff::destroyAndRelease<PathGeometry>(device->resourceAllocator.get(), this, sizeof(std::max_align_t));
}
