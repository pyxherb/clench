#ifndef _CLENCH_GHAL_DEVICE_H_
#define _CLENCH_GHAL_DEVICE_H_

#include "backend.h"
#include "resource.h"
#include "vertex_layout.h"
#include "shader.h"
#include "buffer.h"
#include "texture.h"
#include "views.h"
#include <clench/utils/assert.h>
#include <clench/wsal/window.h>
#include <set>
#include <peff/base/alloc.h>

namespace clench {
	namespace ghal {
		class GHALDeviceContext;

		class GHALDevice {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator, resourceAllocator;
			std::set<GHALDeviceResource *> createdResources;

			CLENCH_NO_COPY_MOVE_METHODS(GHALDevice);

			CLCGHAL_API GHALDevice(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCGHAL_API virtual ~GHALDevice();

			virtual void dealloc() = 0;

			virtual GHALBackend *getBackend() = 0;

			virtual GHALDeviceContext *createDeviceContextForWindow(wsal::Window *window) = 0;

			virtual VertexLayout *createVertexLayout(
				VertexLayoutElementDesc *elementDescs,
				size_t nElementDescs,
				VertexShader *vertexShader) = 0;

			virtual bool isVertexDataTypeSupported(const VertexDataType &vertexDataType) = 0;

			virtual VertexShader *createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) = 0;
			virtual FragmentShader *createFragmentShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) = 0;
			virtual GeometryShader *createGeometryShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) = 0;

			virtual ShaderProgram *linkShaderProgram(Shader **shaders, size_t nShaders) = 0;

			virtual Buffer *createBuffer(const BufferDesc &bufferDesc, const void *initialData) = 0;

			virtual Texture1D *createTexture1D(const char *data, size_t size, const Texture1DDesc &desc) = 0;
			virtual Texture2D *createTexture2D(const char *data, size_t size, const Texture2DDesc &desc) = 0;
			virtual Texture3D *createTexture3D(const char *data, size_t size, const Texture3DDesc &desc) = 0;

			virtual RenderTargetView *createRenderTargetViewForTexture2D(Texture2D *texture) = 0;
		};

		class GHALDeviceContext : public GHALDeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(GHALDeviceContext);

			CLCGHAL_API GHALDeviceContext(GHALDevice *ownerDevice);
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

			virtual void bindVertexLayout(VertexLayout *vertexArray) = 0;

			virtual void setData(Buffer *buffer, const void *data) = 0;

			virtual void setShaderProgram(ShaderProgram *shaderProgram) = 0;

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

			virtual void drawTriangle(unsigned int nTriangles) = 0;
			virtual void drawIndexed(unsigned int nIndices) = 0;

			virtual void present() = 0;
		};
	}
}

#endif
