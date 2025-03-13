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

#define CLCGHAL_DEFINE_SIMPLE_SET_UNIFORM_PARAM(name, type)               \
	struct name##SetUniformParam : public clench::ghal::SetUniformParam { \
		type data;                                                        \
	}

namespace clench {
	namespace ghal {
		class DeviceContext;

		///
		/// @name GCAP List
		/// @{
		constexpr base::UUID
			/// @brief Indicates if the device supports vertex shaders.
			GCAP_VERTEX_SHADER = CLENCH_UUID(a1c48a8d, 068b, 4b2c, bbe2, e68b46168624),
			/// @brief Indicates if the device supports fragment shaders.
			GCAP_FRAGMENT_SHADER = CLENCH_UUID(c5a34f71, b564, 4b6e, acbe, 1a15a7ac2869),
			/// @brief Indicates if the device supports geometry shaders.
			GCAP_GEOMETRY_SHADER = CLENCH_UUID(43a0ccba, 2fc2, 45e2, a2a5, e80e84a8aad7),
			/// @brief Indicates if the device supports hull shaders.
			GCAP_HULL_SHADER = CLENCH_UUID(8eeb55bb, 7974, 4c73, ab69, 5f7c169a9ca8),
			/// @brief Indicates if the device supports domain shaders.
			GCAP_DOMAIN_SHADER = CLENCH_UUID(82b8c34c, 0252, 43c1, aee3, a7a70e27065d),
			/// @brief Indicates if the device supports compute shaders.
			GCAP_COMPUTE_SHADER = CLENCH_UUID(1323c06c, de9b, 4d16, 9e17, 60cdd4fd79cd),

			/// @brief Indicates if the device supports uniform variables.
			GCAP_UNIFORM_VAR = CLENCH_UUID(34ff4211, 48e2, 48f3, 8dda, d08b97d47039),
			/// @brief Indicates if the device supports uniform buffers.
			GCAP_UNIFORM_BUFFER = CLENCH_UUID(cefd2009, f2d6, 4192, b33d, 8649e22a3398),

			/// @brief Indicates if the device supports `int` as a shader element data type.
			GCAP_INT_ELEMENT_TYPE = CLENCH_UUID(9be8d60d, 3979, 4ed1, b51a, 64b76dbec488),
			/// @brief Indicates if the device supports `uint` as a shader element data type.
			GCAP_UINT_ELEMENT_TYPE = CLENCH_UUID(4edccc52, d101, 455a, a603, 6d77081822d2),
			/// @brief Indicates if the device supports `short` as a shader element data type.
			GCAP_SHORT_ELEMENT_TYPE = CLENCH_UUID(a192c300, fd51, 4b7d, 9e7b, 963e39346327),
			/// @brief Indicates if the device supports `ushort` as a shader element data type.
			GCAP_USHORT_ELEMENT_TYPE = CLENCH_UUID(93b905f8, 4131, 4bfc, 98c1, 7e1878b3a2bb),
			/// @brief Indicates if the device supports `long` as a shader element data type.
			GCAP_LONG_ELEMENT_TYPE = CLENCH_UUID(10dadae9, 6be2, 4a1b, acd5, e4651b3a3cef),
			/// @brief Indicates if the device supports `ulong` as a shader element data type.
			GCAP_ULONG_ELEMENT_TYPE = CLENCH_UUID(f07d3ea6, fe2f, 4b1b, a647, 3fcb84a7e784),
			/// @brief Indicates if the device supports `double` as a shader element data type.
			GCAP_DOUBLE_FP_ELEMENT_TYPE = CLENCH_UUID(9ef8a0af, 8d1e, 4a08, ab57, e043f0e82c96),
			/// @brief Indicates if the device supports `half` as a shader element data type.
			GCAP_HALF_FP_ELEMENT_TYPE = CLENCH_UUID(47c7c1bb, 68a8, 464b, 9125, 9b76889f344d);
		///
		/// @}
		///

		using CapabilitySet = peff::Set<base::UUID>;

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
