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

#define CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(name, type) \
	struct name##SetUniformParam : public clench::ghal::SetUniformParam {\
		type data;\
	}

namespace clench {
	namespace ghal {
		class DeviceContext;

		struct GraphicsCapabilities {
			peff::Set<ShaderType> supportedShaderTypes;
			peff::Set<ShaderElementType> supportedElementTypes;
		};

		/// @brief Base SetUniformParam structure, please use the derived structure types.
		struct SetUniformParam {
		};

		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(Int, int);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(UInt, unsigned int);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(Short, short);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(UShort, unsigned short);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(Long, long long);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(ULong, unsigned long long);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(Float, float);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(Double, double);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(Half, uint16_t);
		CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(Boolean, bool);

		class Device {
		public:
			peff::RcObjectPtr<peff::Alloc> selfAllocator, resourceAllocator;
			std::set<DeviceResource *> createdResources;

			CLENCH_NO_COPY_MOVE_METHODS(Device);

			CLCGHAL_API Device(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator);
			CLCGHAL_API virtual ~Device();

			virtual void dealloc() = 0;

			virtual Backend *getBackend() = 0;

			[[nodiscard]] virtual base::ExceptionPtr createDeviceContextForWindow(wsal::Window *window, DeviceContext *&deviceContextOut) = 0;

			virtual base::ExceptionPtr createVertexLayout(
				VertexLayoutElementDesc *elementDescs,
				size_t nElementDescs,
				VertexShader *vertexShader,
				VertexLayout *&vertexLayoutOut) = 0;

			virtual bool isShaderDataTypeSupported(const ShaderDataType &vertexDataType) = 0;

			virtual base::ExceptionPtr createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo, VertexShader *&vertexShaderOut) = 0;
			virtual base::ExceptionPtr createFragmentShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo, FragmentShader *&fragmentShaderOut) = 0;
			virtual base::ExceptionPtr createGeometryShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo, GeometryShader *&geometryShaderOut) = 0;

			virtual base::ExceptionPtr linkShaderProgram(Shader **shaders, size_t nShaders, ShaderProgram *&shaderProgramOut) = 0;

			virtual base::ExceptionPtr createBuffer(const BufferDesc &bufferDesc, const void *initialData, Buffer *&bufferOut) = 0;

			virtual base::ExceptionPtr createTexture1D(const char *data, size_t size, const Texture1DDesc &desc, Texture1D *&textureOut) = 0;
			virtual base::ExceptionPtr createTexture2D(const char *data, size_t size, const Texture2DDesc &desc, Texture2D *&textureOut) = 0;
			virtual base::ExceptionPtr createTexture3D(const char *data, size_t size, const Texture3DDesc &desc, Texture3D *&textureOut) = 0;

			virtual base::ExceptionPtr createRenderTargetViewForTexture2D(Texture2D *texture, RenderTargetView *&renderTargetViewOut) = 0;
		};

		class DeviceContext : public DeviceResource {
		public:
			CLENCH_NO_COPY_MOVE_METHODS(DeviceContext);

			CLCGHAL_API DeviceContext(Device *ownerDevice);
			CLCGHAL_API virtual ~DeviceContext();

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

			/// @brief Set data of a uniform.
			/// @param index Index of the uniform variable.
			/// @param dataType Data type of the uniform variable.
			/// @param setUniformParam Parameter used to set data in the specific type.
			virtual void setUniform(size_t index, const ShaderDataType &dataType, const SetUniformParam &setUniformParam) = 0;
			virtual void setUniformBuffer(Buffer *buffer, size_t index) = 0;

			virtual void drawTriangle(unsigned int nTriangles) = 0;
			virtual void drawIndexed(unsigned int nIndices) = 0;

			virtual void present() = 0;
		};
	}
}

#endif
