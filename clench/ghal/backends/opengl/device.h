#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_DEVICE_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_DEVICE_H_

#include "except.h"
#include "backend.h"
#include "views.h"
#include "shader.h"
// #include "buffer.h"
#include <clench/ghal/device.h>
#include <thread>
#include <optional>
#include <mutex>
#include <clench/utils/logger.h>

namespace clench {
	namespace ghal {
		class GLDeviceContext;

		struct NativeGLContext {
#ifdef _WIN32
			HGLRC wglContext = NULL;
			HWND hWnd = NULL;
			HDC hdc = NULL;
#else
			EGLDisplay eglDisplay = EGL_NO_DISPLAY;
			EGLSurface eglReadSurface = EGL_NO_SURFACE;
			EGLSurface eglDrawSurface = EGL_NO_SURFACE;
			EGLContext eglContext = EGL_NO_CONTEXT;
			EGLConfig eglConfig = NULL;
			EGLNativeWindowType eglWindow;
#endif

			CLCGHAL_API static NativeGLContext saveContextCurrent();
			CLCGHAL_API static bool restoreContextCurrent(const NativeGLContext &context);
			CLCGHAL_API void destroySurface();
			CLCGHAL_API void destroy();
		};

		class GLDevice : public Device {
		public:
			GLBackend *backend;
			/// @brief Default context for some internal operations. DO NOT try to draw with it!
			/// @note DO NOT forget to create one for the device after the creation!
			peff::RcObjectPtr<GLDeviceContext> defaultContext;
			peff::RcObjectPtr<Buffer> pseudoBuffer;

			std::mutex texture1dLock;
			std::mutex texture2dLock;
			std::mutex texture3dLock;

			CLENCH_NO_COPY_MOVE_METHODS(GLDevice);

			CLCGHAL_API GLDevice(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, GLBackend *backend);
			CLCGHAL_API virtual ~GLDevice();

			CLCGHAL_API virtual Backend *getBackend() override;

			CLCGHAL_API virtual base::ExceptionPtr createDeviceContextForWindow(
				clench::wsal::Window *window, DeviceContext *&deviceContextOut) override;

			CLCGHAL_API virtual base::ExceptionPtr createVertexLayout(
				VertexLayoutElementDesc *elementDescs,
				size_t nElementDescs,
				VertexShader *vertexShader,
				VertexLayout *&vertexLayoutOut) override;

			CLCGHAL_API virtual bool isShaderDataTypeSupported(const ShaderDataType &vertexDataType) override;

			CLCGHAL_API virtual base::ExceptionPtr createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo, VertexShader *&vertexShaderOut) override;
			CLCGHAL_API virtual base::ExceptionPtr createFragmentShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo, FragmentShader *&fragmentShaderOut) override;
			CLCGHAL_API virtual base::ExceptionPtr createGeometryShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo, GeometryShader *&geometryShaderOut) override;

			CLCGHAL_API virtual base::ExceptionPtr linkShaderProgram(Shader **shaders, size_t nShaders, ShaderProgram *&shaderProgramOut) override;

			CLCGHAL_API virtual base::ExceptionPtr createBuffer(const BufferDesc &bufferDesc, const void *initialData, Buffer *&bufferOut) override;

			CLCGHAL_API virtual base::ExceptionPtr createTexture1D(const char *data, size_t size, const Texture1DDesc &desc, Texture1D *&textureOut) override;
			CLCGHAL_API virtual base::ExceptionPtr createTexture2D(const char *data, size_t size, const Texture2DDesc &desc, Texture2D *&textureOut) override;
			CLCGHAL_API virtual base::ExceptionPtr createTexture3D(const char *data, size_t size, const Texture3DDesc &desc, Texture3D *&textureOut) override;

			CLCGHAL_API virtual base::ExceptionPtr createRenderTargetViewForTexture2D(Texture2D *texture, RenderTargetView *&renderTargetViewOut) override;

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLDevice *alloc(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, GLBackend *backend);
		};

		class GLDeviceContext : public DeviceContext {
		public:
			peff::RcObjectPtr<GLRenderTargetView> defaultRenderTargetView;
			NativeGLContext nativeGLContext;

			std::optional<std::thread::id> boundThreadId;

			std::optional<NativeGLContext> prevContext;
			std::mutex prevContextSavingMutex;

			std::mutex copyWriteBufferLock;
			std::mutex renderBufferLock;

			GLuint contextLocalVertexArray = GL_NONE;

			int viewportX = 0, viewportY = 0,
				viewportWidth = 0, viewportHeight = 0,
				viewportMinDepth = 0.0f, viewportMaxDepth = 0.0f,
				windowWidth = 0, windowHeight = 0;

			CLENCH_NO_COPY_MOVE_METHODS(GLDeviceContext);

			CLCGHAL_API GLDeviceContext(
				GLDevice *device);
			CLCGHAL_API virtual ~GLDeviceContext();

			CLCGHAL_API virtual RenderTargetView *getDefaultRenderTargetView() override;

			CLCGHAL_API virtual void onResize(int width, int height) override;
			CLCGHAL_API virtual void clearRenderTargetView(
				RenderTargetView *renderTargetView,
				float r,
				float g,
				float b,
				float a) override;
			CLCGHAL_API virtual void clearDepth(
				DepthStencilView *depthStencilView,
				float depth) override;
			CLCGHAL_API virtual void clearStencil(
				DepthStencilView *depthStencilView,
				uint8_t stencil) override;

			CLCGHAL_API virtual void bindVertexBuffer(Buffer *buffer, size_t stride) override;
			CLCGHAL_API virtual void bindIndexBuffer(Buffer *buffer) override;

			CLCGHAL_API virtual void bindVertexLayout(VertexLayout *vertexArray) override;

			CLCGHAL_API virtual void setData(Buffer *buffer, const void *data) override;

			CLCGHAL_API virtual void setShaderProgram(ShaderProgram *shaderProgram) override;

			CLCGHAL_API virtual void setRenderTarget(
				RenderTargetView *renderTargetView,
				DepthStencilView *depthStencilView) override;

			CLCGHAL_API virtual void setViewport(
				int x,
				int y,
				int width,
				int height,
				float minDepth,
				float maxDepth) override;
			CLCGHAL_API virtual void getViewport(
				int &xOut,
				int &yOut,
				int &widthOut,
				int &heightOut,
				float &minDepthOut,
				float &maxDepthOut) override;

			CLCGHAL_API virtual void drawTriangle(unsigned int nTriangles) override;
			CLCGHAL_API virtual void drawIndexed(unsigned int nIndices) override;

			CLCGHAL_API virtual void begin() override;
			CLCGHAL_API virtual void end() override;

			CLCGHAL_API virtual void setUniformBuffer(Buffer *buffer, size_t index) override;

			CLCGHAL_API virtual void present() override;

			CLCGHAL_API virtual void dealloc() override;

			CLCGHAL_API static GLDeviceContext *alloc(GLDevice *device);
		};

		CLCGHAL_API base::ExceptionPtr glErrorToExceptionPtr(GLenum error);
		CLCGHAL_API GLenum toGLVertexDataType(const ShaderDataType &vertexDataType, size_t &sizeOut);
		CLCGHAL_API GLenum toGLTextureFormat(TextureFormat format, GLenum &typeOut);
	}
}

#endif
