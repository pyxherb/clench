#ifndef _CLENCH_GHAL_BACKENDS_OPENGL_DEVICE_H_
#define _CLENCH_GHAL_BACKENDS_OPENGL_DEVICE_H_

#include "backend.h"
#include "views.h"
// #include "buffer.h"
#include <clench/ghal/device.h>

namespace clench {
	namespace ghal {
		class GLGHALDeviceContext;

		struct NativeGLContext {
#ifdef _WIN32
			HGLRC wglContext;
			HDC hdc;
#else
			EGLDisplay eglDisplay;
			EGLSurface eglReadSurface;
			EGLSurface eglDrawSurface;
			EGLContext eglContext;
#endif
		};

		class GLGHALDevice : public GHALDevice {
		public:
			GLGHALBackend *backend;
			/// @brief Default context for some internal operations. DO NOT try to draw with it!
			utils::RcObjectPtr<GLGHALDeviceContext> defaultContext;

			NO_COPY_MOVE_METHODS(GLGHALDevice);

			CLCGHAL_API GLGHALDevice(GLGHALBackend *backend);
			CLCGHAL_API virtual ~GLGHALDevice();

			CLCGHAL_API virtual GHALBackend *getBackend() override;

			CLCGHAL_API virtual GHALDeviceContext *createDeviceContextForWindow(
				clench::wsal::NativeWindow *window) override;

			CLCGHAL_API virtual VertexArray *createVertexArray(
				VertexArrayElementDesc *elementDescs,
				size_t nElementDescs,
				VertexShader *vertexShader) override;

			CLCGHAL_API virtual VertexShader *createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) override;
			CLCGHAL_API virtual FragmentShader *createFragmentShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) override;
			CLCGHAL_API virtual GeometryShader *createGeometryShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) override;

			CLCGHAL_API virtual ShaderProgram *linkShaderProgram(Shader **shaders, size_t nShaders) override;

			CLCGHAL_API virtual Buffer *createBuffer(const BufferDesc &bufferDesc, const void *initialData) override;

			CLCGHAL_API virtual Texture1D *createTexture1D(const char *data, size_t size, const Texture1DDesc &desc) override;
			CLCGHAL_API virtual Texture2D *createTexture2D(const char *data, size_t size, const Texture2DDesc &desc) override;
			CLCGHAL_API virtual Texture3D *createTexture3D(const char *data, size_t size, const Texture3DDesc &desc) override;

			CLCGHAL_API virtual RenderTargetView *createRenderTargetViewForTexture2D(Texture2D *texture) override;
		};

		class GLGHALDeviceContext : public GHALDeviceContext {
		public:
			utils::RcObjectPtr<GLRenderTargetView> defaultRenderTargetView;

#ifdef _WIN32
			HGLRC wglContext;
			HWND hWnd;
			HDC hdc;
#else
			EGLDisplay eglDisplay = EGL_NO_DISPLAY;
			EGLSurface eglSurface = EGL_NO_SURFACE;
			EGLContext eglContext = EGL_NO_CONTEXT;
			EGLConfig eglConfig = nullptr;
			EGLNativeWindowType eglWindow;
#endif
			GLGHALDevice *device;

			int viewportX = 0, viewportY = 0,
				viewportWidth = 0, viewportHeight = 0,
				viewportMinDepth = 0.0f, viewportMaxDepth = 0.0f,
				windowWidth = 0, windowHeight = 0;

			NO_COPY_MOVE_METHODS(GLGHALDeviceContext);

			CLCGHAL_API GLGHALDeviceContext(
				GLGHALDevice *device);
			CLCGHAL_API virtual ~GLGHALDeviceContext();

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

			CLCGHAL_API virtual void bindVertexArray(VertexArray *vertexArray) override;

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

			CLCGHAL_API virtual void drawIndexed(unsigned int nIndices) override;

			CLCGHAL_API virtual void begin() override;
			CLCGHAL_API virtual void end() override;

			CLCGHAL_API virtual void present() override;

			CLCGHAL_API static NativeGLContext saveContextCurrent();
			CLCGHAL_API static bool restoreContextCurrent(const NativeGLContext &context);
			CLCGHAL_API bool makeContextCurrent();
		};

		CLCGHAL_API GLenum toGLTextureFormat(TextureFormat format, GLenum &typeOut);
	}
}

#endif
