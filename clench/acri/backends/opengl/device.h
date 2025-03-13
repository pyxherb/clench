#ifndef _CLENCH_ACRI_BACKENDS_OPENGL_DEVICE_H_
#define _CLENCH_ACRI_BACKENDS_OPENGL_DEVICE_H_

#include <clench/acri/device.h>
#include <clench/ghal/backends/opengl/device.h>

namespace clench {
	namespace acri {
		extern const char g_triangle_solidcolor_fill_vertex_330[];
		extern const size_t g_triangle_solidcolor_fill_vertex_330_length;
		extern const char g_triangle_solidcolor_fill_fragment_330[];
		extern const size_t g_triangle_solidcolor_fill_fragment_330_length;

		extern const char g_ellipse_solidcolor_fill_vertex_330[];
		extern const size_t g_ellipse_solidcolor_fill_vertex_330_length;
		extern const char g_ellipse_solidcolor_fill_fragment_330[];
		extern const size_t g_ellipse_solidcolor_fill_fragment_330_length;

		extern const char g_ellipse_solidcolor_draw_vertex_330[];
		extern const size_t g_ellipse_solidcolor_draw_vertex_330_length;
		extern const char g_ellipse_solidcolor_draw_fragment_330[];
		extern const size_t g_ellipse_solidcolor_draw_fragment_330_length;

		struct TriangleSolidColorFillRenderInfo {
			math::Vec4f color;
		};

		struct EllipseSolidColorFillRenderInfo {
			math::Vec4f color;
			math::Vec2f resolution;
			math::Vec2f offset;
			math::Vec2f origin;
			math::Vec2f radius;
		};

		struct EllipseSolidColorDrawRenderInfo {
			math::Vec4f color;
			math::Vec2f resolution;
			math::Vec2f offset;
			math::Vec2f origin;
			math::Vec2f radius;
			float strokeWidth;
			float transformA, transformB, transformC, transformD, transformE, transformF;
		};

		class GLDevice : public Device {
		public:
			struct DeviceResources {
				peff::RcObjectPtr<ghal::VertexLayout> solidColorFillVertexLayout;
				peff::RcObjectPtr<ghal::VertexLayout> solidColorDrawVertexLayout;
				peff::RcObjectPtr<ghal::ShaderProgram> solidColorFillShaderProgram;
				peff::RcObjectPtr<ghal::ShaderProgram> solidColorEllipseFillShaderProgram;
				peff::RcObjectPtr<ghal::ShaderProgram> solidColorEllipseDrawShaderProgram;
			} deviceResources;

			CLCACRI_API GLDevice(ghal::Device *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCACRI_API ~GLDevice();

			CLCACRI_API virtual void dealloc() override;

			CLCACRI_API static GLDevice *alloc(ghal::Device *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
		};

		class GLDeviceContext : public DeviceContext {
		public:
			struct LocalDeviceResources {
				struct {
					peff::RcObjectPtr<ghal::Buffer> solidColorFillVertexBuffer;
					peff::RcObjectPtr<ghal::Buffer> solidColorFillUniformBuffer;
					std::mutex solidColorFillLock;
				} forTriangle;
				struct {
					peff::RcObjectPtr<ghal::Buffer> solidColorFillVertexBuffer;
					peff::RcObjectPtr<ghal::Buffer> solidColorFillUniformBuffer;
					std::mutex solidColorFillLock;
				} forRect;
				struct {
					peff::RcObjectPtr<ghal::Buffer> solidColorFillVertexBuffer;
					peff::RcObjectPtr<ghal::Buffer> solidColorFillUniformBuffer;
					std::mutex solidColorFillLock;
					peff::RcObjectPtr<ghal::Buffer> solidColorDrawVertexBuffer;
					peff::RcObjectPtr<ghal::Buffer> solidColorDrawUniformBuffer;
					std::mutex solidColorDrawLock;
				} forEllipse;
			} localDeviceResources;

			CLCACRI_API GLDeviceContext(Device *device, ghal::DeviceContext *deviceContext);
			CLCACRI_API ~GLDeviceContext();

			CLCACRI_API virtual void dealloc() override;

			CLCACRI_API virtual void drawTriangle(const TriangleParams &params, Brush *brush, float width) override;
			CLCACRI_API virtual void fillTriangle(const TriangleParams &params, Brush *brush) override;
			CLCACRI_API virtual void drawPolygon(const PolygonParams &params, Brush *brush, float width) override;
			CLCACRI_API virtual void fillPolygon(const PolygonParams &params, Brush *brush) override;
			CLCACRI_API virtual void drawRect(const RectParams &params, Brush *brush, float width) override;
			CLCACRI_API virtual void fillRect(const RectParams &params, Brush *brush) override;
			CLCACRI_API virtual void drawEllipse(const EllipseParams &params, Brush *brush, float width) override;
			CLCACRI_API virtual void fillEllipse(const EllipseParams &params, Brush *brush) override;
			CLCACRI_API virtual void drawPath(const PathParams &params, Brush *brush, float width) override;
			CLCACRI_API virtual void fillPath(const PathParams &params, Brush *brush) override;

			CLCACRI_API static GLDeviceContext *alloc(Device *device, ghal::DeviceContext *deviceContext);
		};

		CLENCH_FORCEINLINE math::Vec2f uiCoordToGLDeviceCoord(float x, float y, float width, float height) {
			return { (x / width) * 2.0f - 1.0f, (1.0f - y / height) * 2.0f - 1.0f };
		}
	}
}

#endif
