#ifndef _CLENCH_ACRI_CONTEXT_H_
#define _CLENCH_ACRI_CONTEXT_H_

#include "resource.h"
#include "geometry.h"
#include "brush.h"
#include <peff/base/alloc.h>
#include <peff/containers/set.h>
#include <peff/containers/hashmap.h>
#include <peff/containers/string.h>
#include <clench/ghal/device.h>

namespace clench {
	namespace acri {
		class DeviceContext;

		class Device {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator, resourceAllocator;
			ghal::Device *associatedDevice;
			peff::Set<ACRIResource *> createdResources;

			CLCACRI_API Device(ghal::Device *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCACRI_API ~Device();

			CLCACRI_API virtual SolidColorBrush *createSolidColorBrush(const ghal::Color &color);

			virtual void dealloc() = 0;
		};

		struct RenderResources {
			struct {
				const char *triangleShaderProgramSrc;
				size_t triangleShaderProgramSize;
			} forTriangle;
		};

		static_assert(std::is_trivial_v<RenderResources>);

		struct TriangleVertex {
			float position[3];
			float color[4];
			float textureCoord[2];
		};

		constexpr ghal::VertexLayoutElementDesc vertexLayoutElementDescs[] = {
			{ clench::ghal::InputVertexShaderSemanticType::Position,
				0,
				{ clench::ghal::ShaderElementType::Float, 3 },
				sizeof(TriangleVertex),
				0 },
			{ clench::ghal::InputVertexShaderSemanticType::Color,
				0,
				{ clench::ghal::ShaderElementType::Float, 4 },
				sizeof(TriangleVertex),
				sizeof(TriangleVertex().position) },
			{ clench::ghal::InputVertexShaderSemanticType::TextureCoord,
				0,
				{ clench::ghal::ShaderElementType::Float, 2 },
				sizeof(TriangleVertex),
				sizeof(TriangleVertex().position) + sizeof(TriangleVertex().color) }
		};

		class DeviceContext : public ACRIResource {
		public:
			peff::RcObjectPtr<ghal::DeviceContext> ghalDeviceContext;

			CLCACRI_API DeviceContext(Device *acriDevice, ghal::DeviceContext *ghalDeviceContext);
			CLCACRI_API ~DeviceContext();

			CLCACRI_API virtual void beginDraw();
			CLCACRI_API virtual void endDraw();

			virtual void drawTriangle(const TriangleParams &params, Brush *brush, float width) = 0;
			virtual void fillTriangle(const TriangleParams &params, Brush *brush) = 0;
			virtual void drawPolygon(const PolygonParams &params, Brush *brush, float width) = 0;
			virtual void fillPolygon(const PolygonParams &params, Brush *brush) = 0;
			virtual void drawRect(const RectParams &params, Brush *brush, float width) = 0;
			virtual void fillRect(const RectParams &params, Brush *brush) = 0;
			virtual void drawEllipse(const EllipseParams &params, Brush *brush, float width) = 0;
			virtual void fillEllipse(const EllipseParams &params, Brush *brush) = 0;
			virtual void drawPath(const PathParams &params, Brush *brush, float width) = 0;
			virtual void fillPath(const PathParams &params, Brush *brush) = 0;
		};

		CLCACRI_API base::ExceptionPtr createDevice(ghal::Device *ghalDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, Device *&deviceOut);
		CLCACRI_API base::ExceptionPtr createDeviceContext(ghal::DeviceContext *ghalDeviceContext, Device *acriDevice, DeviceContext *&deviceContextOut);
	}
}

#endif
