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
		class ACRIDevice : public peff::Deallocable {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator, resourceAllocator;
			ghal::GHALDevice *associatedDevice;
			peff::Set<ACRIResource *> createdResources;

			CLCACRI_API ACRIDevice(ghal::GHALDevice *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCACRI_API ~ACRIDevice();

			CLCACRI_API virtual void dealloc() override;
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
				{ clench::ghal::VertexElementType::Float, 3 },
				sizeof(TriangleVertex),
				0 },
			{ clench::ghal::InputVertexShaderSemanticType::Color,
				0,
				{ clench::ghal::VertexElementType::Float, 4 },
				sizeof(TriangleVertex),
				sizeof(TriangleVertex().position) },
			{ clench::ghal::InputVertexShaderSemanticType::TextureCoord,
				0,
				{ clench::ghal::VertexElementType::Float, 2 },
				sizeof(TriangleVertex),
				sizeof(TriangleVertex().position) + sizeof(TriangleVertex().color) }
		};

		class ACRIDeviceContext : public ACRIResource {
		public:
			peff::RcObjectPtr<ACRIDevice> acriDevice;
			peff::RcObjectPtr<ghal::GHALDeviceContext> ghalDeviceContext;

			CLCACRI_API ACRIDeviceContext(ACRIDevice *acriDevice, ghal::GHALDeviceContext *ghalDeviceContext);
			CLCACRI_API ~ACRIDeviceContext();

			CLCACRI_API virtual void beginDraw();
			CLCACRI_API virtual void endDraw();

			virtual void drawTriangle(TriangleGeometry *geometry, Brush *brush, float width) = 0;
			virtual void fillTriangle(TriangleGeometry *geometry, Brush *brush) = 0;
			virtual void drawEllipse(EllipseGeometry *geometry, Brush *brush, float width) = 0;
			virtual void fillEllipse(EllipseGeometry *geometry, Brush *brush) = 0;
			virtual void drawPath(PathGeometry *geometry, Brush *brush, float width) = 0;
			virtual void fillPath(PathGeometry *geometry, Brush *brush) = 0;
		};
	}
}

#endif
