#ifndef _CLENCH_RHI_DEVICE_H_
#define _CLENCH_RHI_DEVICE_H_

#include "backend.h"
#include "resource.h"
#include "vertex_array.h"
#include "shader.h"
#include <clench/utils/assert.h>
#include <clench/wsi/window.h>
#include <set>

namespace clench {
	namespace rhi {
		class RHIDeviceContext;
		
		class RHIDevice {
		public:
			std::set<RHIDeviceResource*> createdResources;

			NO_COPY_MOVE_METHODS(RHIDevice);

			CLCRHI_API RHIDevice();
			CLCRHI_API virtual ~RHIDevice();

			virtual RHIBackend* getBackend() = 0;

			virtual RHIDeviceContext* createDeviceContextForWindow(wsi::Window* window) = 0;

			virtual VertexArray* createVertexArray(
				VertexArrayElementDesc* elementDescs,
				size_t nElementDescs,
				VertexShader* vertexShader
			) = 0;

			virtual VertexShader *createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) = 0;
			virtual FragmentShader *createFragmentShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) = 0;
			virtual GeometryShader *createGeometryShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) = 0;
		};

		class RHIDeviceContext {
		public:
			NO_COPY_MOVE_METHODS(RHIDeviceContext);

			CLCRHI_API RHIDeviceContext();
			CLCRHI_API virtual ~RHIDeviceContext();

			virtual void onResize(int width, int height) = 0;
			virtual void clear(float r, float g, float b, float a) = 0;
			virtual void clearDepthBuffer(float depth) = 0;
			virtual void clearStencilBuffer(uint8_t stencil) = 0;

			virtual void begin() = 0;
			virtual void end() = 0;

			virtual void present() = 0;
		};
	}
}

#endif
