#ifndef _CLENCH_GHAL_DEVICE_H_
#define _CLENCH_GHAL_DEVICE_H_

#include "backend.h"
#include "resource.h"
#include "vertex_array.h"
#include "shader.h"
#include "buffer.h"
#include "texture.h"
#include "views.h"
#include <clench/utils/assert.h>
#include <clench/wsal/window.h>
#include <set>

namespace clench {
	namespace ghal {
		class GHALDeviceContext;

		class GHALDevice {
		public:
			std::set<GHALDeviceResource *> createdResources;

			NO_COPY_MOVE_METHODS(GHALDevice);

			CLCGHAL_API GHALDevice();
			CLCGHAL_API virtual ~GHALDevice();

			virtual GHALBackend *getBackend() = 0;

			virtual GHALDeviceContext *createDeviceContextForWindow(wsal::NativeWindow *window) = 0;

			virtual VertexArray *createVertexArray(
				VertexArrayElementDesc *elementDescs,
				size_t nElementDescs,
				VertexShader *vertexShader) = 0;

			virtual VertexShader *createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) = 0;
			virtual FragmentShader *createFragmentShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) = 0;
			virtual GeometryShader *createGeometryShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) = 0;

			virtual Buffer *createBuffer(const BufferDesc &bufferDesc, const void *initialData) = 0;

			virtual Texture1D *createTexture1D(const char *data, size_t size, const Texture1DDesc &desc) = 0;
			virtual Texture2D *createTexture2D(const char *data, size_t size, const Texture2DDesc &desc) = 0;
			virtual Texture3D *createTexture3D(const char *data, size_t size, const Texture3DDesc &desc) = 0;

			virtual RenderTargetView *createRenderTargetViewForTexture2D(Texture2D *texture) = 0;
		};

		class GHALDeviceContext {
		public:
			NO_COPY_MOVE_METHODS(GHALDeviceContext);

			CLCGHAL_API GHALDeviceContext();
			CLCGHAL_API virtual ~GHALDeviceContext();

			virtual RenderTargetView *getDefaultRenderTargetView() = 0;

			virtual void onResize(int width, int height) = 0;
			virtual void clearRenderTargetView(
				RenderTargetView *renderTargetView,
				float r,
				float g,
				float b,
				float a) = 0;
			virtual void clearDepth(
				DepthStencilView *depthStencilView,
				float depth) = 0;
			virtual void clearStencil(
				DepthStencilView *depthStencilView,
				uint8_t stencil) = 0;

			virtual void bindVertexBuffer(Buffer *buffer, size_t stride) = 0;
			virtual void bindIndexBuffer(Buffer *buffer) = 0;

			virtual void bindVertexArray(VertexArray *vertexArray) = 0;

			virtual void setData(Buffer *buffer, const void *data) = 0;

			virtual void setVertexShader(VertexShader *vertexShader) = 0;
			virtual void setFragmentShader(FragmentShader *vertexShader) = 0;

			virtual void setRenderTarget(
				RenderTargetView *renderTargetView,
				DepthStencilView *depthStencilView) = 0;

			virtual void setViewport(
				int x,
				int y,
				int width,
				int height,
				float minDepth,
				float maxDepth) = 0;
			virtual void getViewport(
				int &xOut,
				int &yOut,
				int &widthOut,
				int &heightOut,
				float &minDepthOut,
				float &maxDepthOut) = 0;

			virtual void begin() = 0;
			virtual void end() = 0;

			virtual void drawIndexed() = 0;

			virtual void present() = 0;
		};
	}
}

#endif
