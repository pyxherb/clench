#ifndef _CLENCH_ACRI_BACKENDS_OPENGL_H_
#define _CLENCH_ACRI_BACKENDS_OPENGL_H_

#include <clench/acri/device.h>

namespace clench {
	namespace acri {
		class GLACRIDevice : public ACRIDevice {
		public:
			CLCACRI_API GLACRIDevice(ghal::GHALDevice *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCACRI_API ~GLACRIDevice();

			CLCACRI_API static GLACRIDevice *alloc(ghal::GHALDevice *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
		};

		class GLACRIDeviceContext : public ACRIDeviceContext {
		public:
			peff::RcObjectPtr<ghal::GHALDeviceContext> deviceContext;

			CLCACRI_API GLACRIDeviceContext(ACRIDevice *device, ghal::GHALDeviceContext *deviceContext);
			CLCACRI_API ~GLACRIDeviceContext();

			CLCACRI_API virtual void drawTriangle(TriangleGeometry *geometry, Brush *brush, float width) override;
			CLCACRI_API virtual void fillTriangle(TriangleGeometry *geometry, Brush *brush) override;
			CLCACRI_API virtual void drawEllipse(EllipseGeometry *geometry, Brush *brush, float width) override;
			CLCACRI_API virtual void fillEllipse(EllipseGeometry *geometry, Brush *brush) override;
			CLCACRI_API virtual void drawPath(PathGeometry *geometry, Brush *brush, float width) override;
			CLCACRI_API virtual void fillPath(PathGeometry *geometry, Brush *brush) override;
		};
	}
}

#endif
