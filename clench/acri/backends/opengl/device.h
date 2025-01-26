#ifndef _CLENCH_ACRI_BACKENDS_OPENGL_DEVICE_H_
#define _CLENCH_ACRI_BACKENDS_OPENGL_DEVICE_H_

#include <clench/acri/device.h>

namespace clench {
	namespace acri {
		extern const char *g_triangle_solidcolor_vertex_330;
		extern const size_t g_triangle_solidcolor_vertex_330_len;
		extern const char *g_triangle_solidcolor_fragment_330;
		extern const size_t g_triangle_solidcolor_fragment_330_len;

		class GLACRIDevice : public ACRIDevice {
		public:
			struct DeviceResources {
				struct {
					peff::RcObjectPtr<ghal::VertexLayout> solidColorVertexLayout;
					peff::RcObjectPtr<ghal::ShaderProgram> solidColorShaderProgram;
				} forTriangle;
			} deviceResources;

			CLCACRI_API GLACRIDevice(ghal::GHALDevice *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCACRI_API ~GLACRIDevice();

			CLCACRI_API virtual void dealloc() override;

			CLCACRI_API static GLACRIDevice *alloc(ghal::GHALDevice *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
		};

		class GLACRIDeviceContext : public ACRIDeviceContext {
		public:
			peff::RcObjectPtr<ghal::GHALDeviceContext> deviceContext;

			struct LocalDeviceResources {
				struct {
					peff::RcObjectPtr<ghal::Buffer> solidColorVertexBuffer;
					std::mutex solidColorLock;
				} forTriangle;
			} localDeviceResources;

			CLCACRI_API GLACRIDeviceContext(ACRIDevice *device, ghal::GHALDeviceContext *deviceContext);
			CLCACRI_API ~GLACRIDeviceContext();

			CLCACRI_API virtual void dealloc() override;

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
