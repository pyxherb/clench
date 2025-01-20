#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API void GLACRIDeviceContext::drawTriangle(TriangleGeometry *geometry, Brush *brush, float width) {
	switch (brush->brushType) {
		case BrushType::SolidColor: {
			break;
		}
	}
}

CLCACRI_API void GLACRIDeviceContext::fillTriangle(TriangleGeometry *geometry, Brush *brush) {
}

CLCACRI_API void GLACRIDeviceContext::drawEllipse(EllipseGeometry *geometry, Brush *brush, float width) {
}

CLCACRI_API void GLACRIDeviceContext::fillEllipse(EllipseGeometry *geometry, Brush *brush) {
}

CLCACRI_API void GLACRIDeviceContext::drawPath(PathGeometry *geometry, Brush *brush, float width) {
}

CLCACRI_API void GLACRIDeviceContext::fillPath(PathGeometry *geometry, Brush *brush) {
}
