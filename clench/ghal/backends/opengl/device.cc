#include "device.h"
#include "vertex_layout.h"
#include "shader.h"
#include "buffer.h"
#include "views.h"
#include "texture.h"

#if _WIN32
	#include <clench/wsal/backends/win32/window.h>
#endif

#if __unix__
	#include <clench/wsal/backends/x11/window.h>
#endif

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLDevice::GLDevice(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, GLGHALBackend *backend)
	: Device(selfAllocator, resourceAllocator),
	  backend(backend) {
}

CLCGHAL_API GLDevice::~GLDevice() {
}

CLCGHAL_API GHALBackend *GLDevice::getBackend() {
	return backend;
}

CLCGHAL_API base::ExceptionPtr GLDevice::createDeviceContextForWindow(clench::wsal::Window *window, DeviceContext *&deviceContextOut) {
	int width, height;
	window->getSize(width, height);

	std::unique_ptr<
		GLDeviceContext,
		peff::RcObjectUniquePtrDeleter>
		deviceContext(GLDeviceContext::alloc(this));
	if (!deviceContext)
		return nullptr;
#ifdef _WIN32
	deviceContext->hWnd = ((wsal::Win32Window *)window)->nativeHandle;
	deviceContext->hdc = GetDC(deviceContext->hWnd);
	{
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;

		auto pxFmt = ChoosePixelFormat(deviceContext->hdc, &pfd);
		if (!pxFmt)
			throw std::runtime_error("Incompatible ghal device");

		SetPixelFormat(deviceContext->hdc, pxFmt, &pfd);
	}

	if (!(deviceContext->wglContext = wglCreateContext(deviceContext->hdc)))
		throw std::runtime_error("Error creating WGL context");

	NativeGLContext prevContext = GLDeviceContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		GLDeviceContext::restoreContextCurrent(prevContext);
	});
	deviceContext->makeContextCurrent();

	if (!g_glInitialized) {
		int version = gladLoadGL((GLADloadfunc)_loadGlProc);
		if (!version)
			throw std::runtime_error("Error initializing GLAD");
		g_glInitialized = true;
	}
#else
	static const EGLint configAttribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE
	};

	EGLint eglMinor, eglMajor;

	deviceContext->nativeGLContext.eglDisplay = eglGetDisplay((EGLNativeDisplayType)((wsal::X11Window *)window)->nativeHandle.display);
	if (auto it = backend->initializedEglDisplays.find(deviceContext->nativeGLContext.eglDisplay); it != backend->initializedEglDisplays.end()) {
		++it.value();
	} else {
		if(!backend->initializedEglDisplays.insert(+deviceContext->nativeGLContext.eglDisplay, 1))
			return base::OutOfMemoryException::alloc();
		eglInitialize(deviceContext->nativeGLContext.eglDisplay, &eglMajor, &eglMinor);
	}

	deviceContext->nativeGLContext.eglWindow = (EGLNativeWindowType)((wsal::X11Window *)window)->nativeHandle.window;

	EGLint nConfigs;
	eglChooseConfig(deviceContext->nativeGLContext.eglDisplay, configAttribs, &deviceContext->nativeGLContext.eglConfig, 1, &nConfigs);

	if ((deviceContext->nativeGLContext.eglReadSurface =
				(deviceContext->nativeGLContext.eglDrawSurface = eglCreateWindowSurface(
					 deviceContext->nativeGLContext.eglDisplay,
					 deviceContext->nativeGLContext.eglConfig,
					 deviceContext->nativeGLContext.eglWindow,
					 nullptr))) == EGL_NO_SURFACE)
		return base::wrapIfExceptAllocFailed(
			ErrorCreatingDeviceContextException::alloc(
				resourceAllocator.get(),
				base::wrapIfExceptAllocFailed(ErrorCreatingEGLSurfaceException::alloc(resourceAllocator.get()))));

	eglBindAPI(EGL_OPENGL_API);

	static EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	deviceContext->nativeGLContext.eglContext = eglCreateContext(deviceContext->nativeGLContext.eglDisplay, deviceContext->nativeGLContext.eglConfig, EGL_NO_CONTEXT, contextAttribs);

	NativeGLContext prevContext = NativeGLContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		NativeGLContext::restoreContextCurrent(prevContext);
	});
	NativeGLContext::restoreContextCurrent(deviceContext->nativeGLContext);

	if (!g_glBackend) {
		// The initialization of GLAD is deferred because the `_loadGlProc`
		// requires EGL and we can only initialize EGL with a specific display.
		int version = gladLoadGL((GLADloadfunc)_loadGlProc);
		if (!version)
			return base::wrapIfExceptAllocFailed(
				ErrorCreatingDeviceContextException::alloc(
					resourceAllocator.get(),
					base::wrapIfExceptAllocFailed(ErrorInitializingGLLoaderException::alloc(resourceAllocator.get()))));
		g_glInitialized = true;
	}
#endif
	GLint defaultFramebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&defaultFramebuffer);
	deviceContext->defaultRenderTargetView = GLRenderTargetView::alloc(this, RenderTargetViewType::Buffer, defaultFramebuffer);

	deviceContext->onResize(width, height);

	deviceContext->incRef();
	deviceContextOut = deviceContext.release();

	return {};
}

CLCGHAL_API base::ExceptionPtr GLDevice::createVertexLayout(
	VertexLayoutElementDesc *elementDescs,
	size_t nElementDescs,
	VertexShader *vertexShader,
	VertexLayout *&vertexLayoutOut) {
	GLuint vao;
	glGenVertexArrays(1, &vao);

	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	peff::ScopeGuard deleteVaoGuard([&vao]() noexcept {
		glDeleteVertexArrays(1, &vao);
	});

	GLint prevVao;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVao);
	peff::ScopeGuard restoreTextureGuard([prevVao]() noexcept {
		glBindVertexArray(prevVao);
	});

	std::unique_ptr<GLVertexLayout, peff::RcObjectUniquePtrDeleter> vertexArray(GLVertexLayout::alloc(this, vao));
	if (!vertexArray)
		return base::OutOfMemoryException::alloc();

	for (size_t i = 0; i < nElementDescs; ++i) {
		VertexLayoutElementDesc &curDesc = elementDescs[i];

		glEnableVertexArrayAttrib(vao, i);

		size_t sizeOut;
		GLenum glType;
		if ((glType = toGLVertexDataType(curDesc.dataType, sizeOut)) == GL_INVALID_ENUM) {
			return base::wrapIfExceptAllocFailed(InvalidVertexDataTypeException::alloc(resourceAllocator.get(), i));
		}

		glVertexAttribPointer(i, sizeOut * curDesc.dataType.nElements, glType, false, curDesc.stride, (void *)curDesc.off);
	}

	deleteVaoGuard.release();

	vertexArray->incRef();
	vertexLayoutOut = vertexArray.release();

	return {};
}

CLCGHAL_API bool GLDevice::isVertexDataTypeSupported(const VertexDataType &vertexDataType) {
	return true;
}

CLCGHAL_API base::ExceptionPtr GLDevice::createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo, VertexShader *&vertexShaderOut) {
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	peff::ScopeGuard deleteShaderGuard([shader]() noexcept {
		glDeleteShader(shader);
	});

	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	glShaderSource(shader, 1, &source, (GLint *)&size);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	glCompileShader(shader);

	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLsizei size;
		glGetShaderInfoLog(shader, 0, &size, NULL);

		peff::String log;
		if (!log.resize(size)) {
			return base::OutOfMemoryException::alloc();
		}

		glGetShaderInfoLog(shader, 1024, &size, log.data());
		return base::wrapIfExceptAllocFailed(
			ErrorCompilingShaderException::alloc(
				resourceAllocator.get(),
				std::move(log)));
	}

	std::unique_ptr<GLVertexShader, peff::RcObjectUniquePtrDeleter> vertexShader(GLVertexShader::alloc(this, shader));
	if (!vertexShader)
		return base::OutOfMemoryException::alloc();

	deleteShaderGuard.release();

	vertexShader->incRef();
	vertexShaderOut = vertexShader.release();

	return {};
}

CLCGHAL_API base::ExceptionPtr GLDevice::createFragmentShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo, FragmentShader *&fragmentShaderOut) {
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	peff::ScopeGuard deleteShaderGuard([shader]() noexcept {
		glDeleteShader(shader);
	});

	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	glShaderSource(shader, 1, &source, (GLint *)&size);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	glCompileShader(shader);

	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLsizei size;
		glGetShaderInfoLog(shader, 0, &size, NULL);

		peff::String log;
		if (!log.resize(size)) {
			return base::OutOfMemoryException::alloc();
		}

		glGetShaderInfoLog(shader, 1024, &size, log.data());
		return base::wrapIfExceptAllocFailed(
			ErrorCompilingShaderException::alloc(
				resourceAllocator.get(),
				std::move(log)));
	}

	std::unique_ptr<GLFragmentShader, peff::RcObjectUniquePtrDeleter> fragmentShader(GLFragmentShader::alloc(this, shader));
	if (!fragmentShader)
		return base::OutOfMemoryException::alloc();

	deleteShaderGuard.release();

	fragmentShader->incRef();
	fragmentShaderOut = fragmentShader.release();

	return {};
}

CLCGHAL_API base::ExceptionPtr GLDevice::createGeometryShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo, GeometryShader *&geometryShaderOut) {
	return {};
}

CLCGHAL_API base::ExceptionPtr GLDevice::linkShaderProgram(Shader **shaders, size_t nShaders, ShaderProgram *&shaderProgramOut) {
	GLVertexShader *vertexShader = nullptr;
	GLFragmentShader *fragmentShader = nullptr;

	for (size_t i = 0; i < nShaders; ++i) {
		Shader *curShader = shaders[i];

		switch (curShader->shaderType) {
			case ShaderType::Vertex: {
				if (vertexShader) {
					return base::wrapIfExceptAllocFailed(
						DuplicatedShaderPartException::alloc(resourceAllocator.get(), i));
				}
				vertexShader = (GLVertexShader *)curShader;
				break;
			}
			case ShaderType::Fragment: {
				if (fragmentShader) {
					return base::wrapIfExceptAllocFailed(
						DuplicatedShaderPartException::alloc(resourceAllocator.get(), i));
				}
				fragmentShader = (GLFragmentShader *)curShader;
				break;
			}
			default:
				break;
		}
	}

	if (!vertexShader)
		return base::wrapIfExceptAllocFailed(
			MissingShaderPartException::alloc(resourceAllocator.get(), ShaderType::Vertex));

	if (!fragmentShader)
		return base::wrapIfExceptAllocFailed(
			MissingShaderPartException::alloc(resourceAllocator.get(), ShaderType::Fragment));

	GLuint program = glCreateProgram();
	peff::ScopeGuard deleteProgramGuard([program]() noexcept {
		glDeleteProgram(program);
	});

	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	glAttachShader(program, vertexShader->shaderHandle);
	glAttachShader(program, fragmentShader->shaderHandle);

	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	GLint success;
	glLinkProgram(program);

	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLsizei size;
		glGetProgramInfoLog(program, 0, &size, NULL);

		peff::String log;
		if (!log.resize(size)) {
			return base::OutOfMemoryException::alloc();
		}

		glGetProgramInfoLog(program, 1024, &size, log.data());
		return base::wrapIfExceptAllocFailed(
			ErrorLinkingShaderException::alloc(
				resourceAllocator.get(),
				std::move(log)));
	}

	std::unique_ptr<GLShaderProgram, peff::RcObjectUniquePtrDeleter> shaderProgram(GLShaderProgram::alloc(this, program));
	if (!shaderProgram)
		return nullptr;

	deleteProgramGuard.release();

	shaderProgram->incRef();
	shaderProgramOut = shaderProgram.release();

	return {};
}

CLCGHAL_API base::ExceptionPtr GLDevice::createBuffer(const BufferDesc &bufferDesc, const void *initialData, Buffer *&bufferOut) {
	GLuint buffer;
	glGenBuffers(1, &buffer);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	peff::ScopeGuard deleteBufferGuard([buffer]() noexcept {
		glDeleteBuffers(1, &buffer);
	});

	std::unique_ptr<GLBuffer, peff::RcObjectUniquePtrDeleter> glBuffer(GLBuffer::alloc(this, bufferDesc, buffer));
	if (!glBuffer)
		return base::OutOfMemoryException::alloc();

	deleteBufferGuard.release();

	defaultContext->setData(glBuffer.get(), initialData);

	glBuffer->incRef();
	bufferOut = glBuffer.release();
	return {};
}

CLCGHAL_API base::ExceptionPtr GLDevice::createTexture1D(const char *data, size_t size, const Texture1DDesc &desc, Texture1D *&textureOut) {
	GLenum glType;
	GLenum glTextureFormat = toGLTextureFormat(desc.format, glType);
	if (glTextureFormat == GL_INVALID_ENUM)
		throw std::runtime_error("Invalid texture format");

	GLuint texture;
	glGenTextures(1, &texture);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	peff::ScopeGuard deleteTextureGuard([texture]() noexcept {
		glDeleteTextures(1, &texture);
	});

	texture1dLock.lock();
	GLint savedTexture;
	glGetIntegerv(GL_TEXTURE_1D, &savedTexture);
	peff::ScopeGuard restoreTextureGuard([this, savedTexture]() noexcept {
		glBindTexture(GL_TEXTURE_1D, savedTexture);
		texture1dLock.unlock();
	});

	glBindTexture(GL_TEXTURE_1D, texture);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage1D(
		GL_TEXTURE_1D,
		0,
		glTextureFormat,
		desc.width,
		0,
		glTextureFormat,
		glType,
		data);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	std::unique_ptr<GLTexture1D, peff::RcObjectUniquePtrDeleter> texture1d(GLTexture1D::alloc(this, desc, texture));
	if (!texture1d)
		return base::OutOfMemoryException::alloc();

	deleteTextureGuard.release();

	texture1d->incRef();
	textureOut = texture1d.release();
	return {};
}

CLCGHAL_API base::ExceptionPtr GLDevice::createTexture2D(const char *data, size_t size, const Texture2DDesc &desc, Texture2D *&textureOut) {
	GLenum glType;
	GLenum glTextureFormat = toGLTextureFormat(desc.format, glType);
	if (glTextureFormat == GL_INVALID_ENUM)
		throw std::runtime_error("Invalid texture format");

	GLuint texture;
	glGenTextures(1, &texture);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	peff::ScopeGuard deleteTextureGuard([texture]() noexcept {
		glDeleteTextures(1, &texture);
	});

	texture2dLock.lock();
	GLint savedTexture;
	glGetIntegerv(GL_TEXTURE_2D, &savedTexture);
	peff::ScopeGuard restoreTextureGuard([this, savedTexture]() noexcept {
		glBindTexture(GL_TEXTURE_2D, savedTexture);
		texture2dLock.unlock();
	});

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		glTextureFormat,
		desc.width,
		desc.height,
		0,
		glTextureFormat,
		glType,
		data);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	std::unique_ptr<GLTexture2D, peff::RcObjectUniquePtrDeleter> texture2d(GLTexture2D::alloc(this, desc, texture));
	if (!texture2d)
		return base::OutOfMemoryException::alloc();

	deleteTextureGuard.release();

	texture2d->incRef();
	textureOut = texture2d.release();
	return {};
}

CLCGHAL_API base::ExceptionPtr GLDevice::createTexture3D(const char *data, size_t size, const Texture3DDesc &desc, Texture3D *&textureOut) {
	GLenum glType;
	GLenum glTextureFormat = toGLTextureFormat(desc.format, glType);
	if (glTextureFormat == GL_INVALID_ENUM)
		throw std::runtime_error("Invalid texture format");

	GLuint texture;
	glGenTextures(1, &texture);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	peff::ScopeGuard deleteTextureGuard([texture]() noexcept {
		glDeleteTextures(1, &texture);
	});

	texture3dLock.lock();
	GLuint savedTexture;
	glGetIntegerv(GL_TEXTURE_3D, (GLint *)&savedTexture);
	peff::ScopeGuard restoreTextureGuard([this, savedTexture]() noexcept {
		glBindTexture(GL_TEXTURE_3D, savedTexture);
		texture3dLock.unlock();
	});

	glBindTexture(GL_TEXTURE_3D, texture);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage3D(
		GL_TEXTURE_3D,
		0,
		glTextureFormat,
		desc.width,
		desc.height,
		desc.depth,
		0,
		glTextureFormat,
		glType,
		data);
	CLENCH_RETURN_IF_EXCEPT(glErrorToExceptionPtr(glGetError()));

	std::unique_ptr<GLTexture3D, peff::RcObjectUniquePtrDeleter> texture3d(GLTexture3D::alloc(this, desc, texture));
	if (!texture3d)
		return base::OutOfMemoryException::alloc();

	deleteTextureGuard.release();

	texture3d->incRef();
	textureOut = texture3d.release();
	return {};
}

CLCGHAL_API RenderTargetView *GLDevice::createRenderTargetViewForTexture2D(Texture2D *texture) {
	return GLRenderTargetView::alloc(this, RenderTargetViewType::Texture2D, ((GLTexture2D *)texture)->textureHandle);
}

CLCGHAL_API void GLDevice::dealloc() {
	peff::destroyAndRelease<GLDevice>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API GLDevice *GLDevice::alloc(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, GLGHALBackend *backend) {
	std::unique_ptr<GLDevice, peff::DeallocableDeleter<GLDevice>> ptr(
		peff::allocAndConstruct<GLDevice>(
			selfAllocator, sizeof(std::max_align_t),
			selfAllocator, resourceAllocator, backend));
	if (!ptr)
		return nullptr;

	if (!(ptr->defaultContext = GLDeviceContext::alloc(ptr.get())))
		return nullptr;

	return (GLDevice *)ptr.release();
}

CLCGHAL_API GLDeviceContext::GLDeviceContext(
	GLDevice *device)
	: DeviceContext(device) {
}

CLCGHAL_API GLDeviceContext::~GLDeviceContext() {
	nativeGLContext.destroy();
}

CLCGHAL_API RenderTargetView *GLDeviceContext::getDefaultRenderTargetView() {
	return defaultRenderTargetView.get();
}

CLCGHAL_API void GLDeviceContext::onResize(int width, int height) {
	NativeGLContext prevContext = NativeGLContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		NativeGLContext::restoreContextCurrent(prevContext);
	});

#ifdef _WIN32
#else
	windowWidth = width;
	windowHeight = height;

	nativeGLContext.destroySurface();

	if ((nativeGLContext.eglReadSurface =
				(nativeGLContext.eglDrawSurface =
						eglCreateWindowSurface(nativeGLContext.eglDisplay,
							nativeGLContext.eglConfig,
							nativeGLContext.eglWindow,
							nullptr))) == EGL_NO_SURFACE)
		throw std::runtime_error("Error creating EGL surface");
#endif
}

CLCGHAL_API void GLDeviceContext::clearRenderTargetView(
	RenderTargetView *renderTargetView,
	float r,
	float g,
	float b,
	float a) {
	NativeGLContext prevContext = NativeGLContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		NativeGLContext::restoreContextCurrent(prevContext);
	});

	GLuint prevRenderTarget;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&prevRenderTarget);

	peff::ScopeGuard restoreRenderTargetGuard([prevRenderTarget]() noexcept {
		glBindFramebuffer(GL_FRAMEBUFFER, prevRenderTarget);
	});

	if (!renderTargetView)
		renderTargetView = defaultRenderTargetView.get();

	glBindFramebuffer(GL_FRAMEBUFFER, ((GLRenderTargetView *)renderTargetView)->frameBufferHandle);
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

CLCGHAL_API void GLDeviceContext::clearDepth(
	DepthStencilView *depthStencilView,
	float depth) {
	NativeGLContext prevContext = NativeGLContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		NativeGLContext::restoreContextCurrent(prevContext);
	});

	GLuint prevRenderTarget;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&prevRenderTarget);

	peff::ScopeGuard restoreRenderTargetGuard([prevRenderTarget]() noexcept {
		glBindFramebuffer(GL_FRAMEBUFFER, prevRenderTarget);
	});

	glBindFramebuffer(GL_FRAMEBUFFER, ((GLDepthStencilView *)depthStencilView)->frameBufferHandle);
	glDepthMask(GL_TRUE);
	glClearDepth(depth);
	glClear(GL_DEPTH_BUFFER_BIT);
}

CLCGHAL_API void GLDeviceContext::clearStencil(
	DepthStencilView *depthStencilView,
	uint8_t stencil) {
	NativeGLContext prevContext = NativeGLContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		NativeGLContext::restoreContextCurrent(prevContext);
	});

	GLuint prevRenderTarget;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&prevRenderTarget);

	peff::ScopeGuard restoreRenderTargetGuard([prevRenderTarget]() noexcept {
		glBindFramebuffer(GL_FRAMEBUFFER, prevRenderTarget);
	});

	glBindFramebuffer(GL_FRAMEBUFFER, ((GLDepthStencilView *)depthStencilView)->frameBufferHandle);
	glStencilMask(0xff);
	glClearStencil(stencil);
	glClear(GL_STENCIL_BUFFER_BIT);
}

CLCGHAL_API void GLDeviceContext::bindVertexBuffer(Buffer *buffer, size_t stride) {
	NativeGLContext prevContext = NativeGLContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		NativeGLContext::restoreContextCurrent(prevContext);
	});

	glBindBuffer(GL_ARRAY_BUFFER, ((GLBuffer *)buffer)->bufferHandle);
}

CLCGHAL_API void GLDeviceContext::bindIndexBuffer(Buffer *buffer) {
	NativeGLContext prevContext = NativeGLContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		NativeGLContext::restoreContextCurrent(prevContext);
	});

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLBuffer *)buffer)->bufferHandle);
}

CLCGHAL_API void GLDeviceContext::bindVertexLayout(VertexLayout *vertexArray) {
	NativeGLContext prevContext = NativeGLContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() noexcept {
		NativeGLContext::restoreContextCurrent(prevContext);
	});

	glBindVertexArray(((GLVertexLayout *)vertexArray)->vertexArrayHandle);
}

CLCGHAL_API void GLDeviceContext::setData(Buffer *buffer, const void *data) {
	copyWriteBufferLock.lock();
	GLuint prevBuffer;
	glGetIntegerv(GL_COPY_WRITE_BUFFER, (GLint *)&prevBuffer);
	peff::ScopeGuard restoreBufferGuard([this, prevBuffer]() noexcept {
		copyWriteBufferLock.unlock();
		glBindBuffer(GL_COPY_WRITE_BUFFER, prevBuffer);
	});

	glBindBuffer(GL_COPY_WRITE_BUFFER, ((GLBuffer *)buffer)->bufferHandle);

	GLenum usage;
	switch (buffer->bufferDesc.usage) {
		case BufferUsage::Static: {
			if (buffer->bufferDesc.cpuWritable) {
				usage = GL_STATIC_COPY;
			} else if (buffer->bufferDesc.cpuReadable) {
				usage = GL_STATIC_READ;
			} else {
				usage = GL_STATIC_DRAW;
			}
			break;
		}
		case BufferUsage::Default:
		case BufferUsage::Dynamic: {
			if (buffer->bufferDesc.cpuWritable) {
				usage = GL_DYNAMIC_COPY;
			} else if (buffer->bufferDesc.cpuReadable) {
				usage = GL_DYNAMIC_READ;
			} else {
				usage = GL_DYNAMIC_DRAW;
			}
			break;
		}
		case BufferUsage::Staging: {
			if (buffer->bufferDesc.cpuWritable) {
				usage = GL_STREAM_COPY;
			} else if (buffer->bufferDesc.cpuReadable) {
				usage = GL_STREAM_READ;
			} else {
				usage = GL_STREAM_DRAW;
			}
			break;
		}
	}

	glBufferData(GL_COPY_WRITE_BUFFER, buffer->bufferDesc.size, data, usage);
}

CLCGHAL_API void GLDeviceContext::setShaderProgram(ShaderProgram *shaderProgram) {
	glUseProgram(((GLShaderProgram *)shaderProgram)->programHandle);
}

CLCGHAL_API void GLDeviceContext::setRenderTarget(
	RenderTargetView *renderTargetView,
	DepthStencilView *depthStencilView) {
	glBindFramebuffer(GL_FRAMEBUFFER, ((GLRenderTargetView *)renderTargetView)->frameBufferHandle);

	renderBufferLock.lock();
	GLuint prevRenderTarget;
	glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint *)&prevRenderTarget);

	peff::ScopeGuard restoreRenderTargetGuard([this, prevRenderTarget]() noexcept {
		glBindRenderbuffer(GL_RENDERBUFFER, prevRenderTarget);
		renderBufferLock.unlock();
	});

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ((GLDepthStencilView *)depthStencilView)->frameBufferHandle);
}

CLCGHAL_API void GLDeviceContext::setViewport(
	int x,
	int y,
	int width,
	int height,
	float minDepth,
	float maxDepth) {
	viewportX = x,
	viewportY = y;
	viewportWidth = width,
	viewportHeight = height;
	viewportMinDepth = minDepth;
	viewportMaxDepth = maxDepth;

	glScissor(x, (windowHeight - y - height), width, height);
	glViewport(x, (windowHeight - y - height), width, height);
	glDepthRange(minDepth, maxDepth);
}

CLCGHAL_API void GLDeviceContext::getViewport(
	int &xOut,
	int &yOut,
	int &widthOut,
	int &heightOut,
	float &minDepthOut,
	float &maxDepthOut) {
	xOut = viewportX;
	yOut = viewportY;
	widthOut = viewportWidth;
	heightOut = viewportHeight;
	minDepthOut = viewportMinDepth;
	maxDepthOut = viewportMaxDepth;
}

CLCGHAL_API void GLDeviceContext::drawTriangle(unsigned int nTriangles) {
	glDrawArrays(GL_TRIANGLES, 0, nTriangles);
}

CLCGHAL_API void GLDeviceContext::drawIndexed(unsigned int nIndices) {
	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, nullptr);
}

CLCGHAL_API void GLDeviceContext::begin() {
	NativeGLContext::restoreContextCurrent(nativeGLContext);
}

CLCGHAL_API void GLDeviceContext::end() {
}

CLCGHAL_API void GLDeviceContext::present() {
#ifdef _WIN32
	SwapBuffers(hdc);
#else
	eglSwapBuffers(nativeGLContext.eglDisplay, nativeGLContext.eglDrawSurface);
#endif
}

CLCGHAL_API NativeGLContext NativeGLContext::saveContextCurrent() {
#if _WIN32
	return {
		wglGetCurrentContext(),
		wglGetCurrentDC()
	};
#else
	return { eglGetCurrentDisplay(),
		eglGetCurrentSurface(EGL_DRAW),
		eglGetCurrentSurface(EGL_READ),
		eglGetCurrentContext() };
#endif
}

CLCGHAL_API bool NativeGLContext::restoreContextCurrent(const NativeGLContext &context) {
#if _WIN32
	return wglMakeCurrent(context.hdc, context.wglContext);
#else
	return eglMakeCurrent(context.eglDisplay, context.eglDrawSurface, context.eglReadSurface, context.eglContext);
#endif
}

CLCGHAL_API void NativeGLContext::destroySurface() {
	if (eglDrawSurface != eglReadSurface) {
		if (eglDrawSurface != EGL_NO_SURFACE) {
			eglDestroySurface(eglDisplay, eglDrawSurface);
		}

		if (eglReadSurface != EGL_NO_SURFACE) {
			eglDestroySurface(eglDisplay, eglReadSurface);
		}
	} else {
		if (eglDrawSurface != EGL_NO_SURFACE) {
			eglDestroySurface(eglDisplay, eglDrawSurface);
		}
	}
}

CLCGHAL_API void NativeGLContext::destroy() {
#if _WIN32
	if (wglContext)
		wglDeleteContext(wglContext);
	if (hdc)
		ReleaseDC(hWnd, hdc);
#else
	if (eglContext != EGL_NO_CONTEXT) {
		eglDestroyContext(eglDisplay, eglContext);
	}

	destroySurface();

	if (eglDisplay != EGL_NO_DISPLAY) {
		if (auto it = g_glBackend->initializedEglDisplays.find(eglDisplay); it != g_glBackend->initializedEglDisplays.end()) {
			if (!--it.value()) {
				eglTerminate(eglDisplay);
				g_glBackend->initializedEglDisplays.remove(it);
			}
		} else {
			eglTerminate(eglDisplay);
		}
	}
#endif
}

CLCGHAL_API GLDeviceContext *GLDeviceContext::alloc(GLDevice *device) {
	return peff::allocAndConstruct<GLDeviceContext>(
		device->resourceAllocator.get(), sizeof(std::max_align_t),
		device);
}

CLCGHAL_API void GLDeviceContext::dealloc() {
	peff::destroyAndRelease<GLDeviceContext>(ownerDevice->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API base::ExceptionPtr clench::ghal::glErrorToExceptionPtr(GLenum error) {
	switch (error) {
		case GL_NO_ERROR:
			break;
		case GL_OUT_OF_MEMORY:
			return clench::base::OutOfMemoryException::alloc();
	}

	return {};
}

CLCGHAL_API GLenum clench::ghal::toGLVertexDataType(const VertexDataType &vertexDataType, size_t &sizeOut) {
	switch (vertexDataType.elementType) {
		case VertexElementType::Int:
			sizeOut = sizeof(GLint) * vertexDataType.nElements;
			return GL_INT;
		case VertexElementType::UInt:
			sizeOut = sizeof(GLuint) * vertexDataType.nElements;
			return GL_UNSIGNED_INT;
		case VertexElementType::Short:
			sizeOut = sizeof(GLshort) * vertexDataType.nElements;
			return GL_SHORT;
		case VertexElementType::UShort:
			sizeOut = sizeof(GLushort) * vertexDataType.nElements;
			return GL_UNSIGNED_SHORT;
		case VertexElementType::Float:
			sizeOut = sizeof(GLfloat) * vertexDataType.nElements;
			return GL_FLOAT;
		case VertexElementType::Double:
			sizeOut = sizeof(GLdouble) * vertexDataType.nElements;
			return GL_DOUBLE;
		case VertexElementType::Boolean:
			sizeOut = sizeof(GLboolean) * vertexDataType.nElements;
			return GL_BOOL;
		default:
			return GL_INVALID_ENUM;
	}
}

CLCGHAL_API GLenum clench::ghal::toGLTextureFormat(TextureFormat format, GLenum &typeOut) {
	switch (format) {
		case TextureFormat::RGB8:
			typeOut = GL_UNSIGNED_BYTE;
			return GL_RGB;
		case TextureFormat::RGBA8:
			typeOut = GL_UNSIGNED_BYTE;
			return GL_RGBA;
		case TextureFormat::RGB32F:
			typeOut = GL_FLOAT;
			return GL_RGB;
		case TextureFormat::RGBA32F:
			typeOut = GL_FLOAT;
			return GL_RGBA;
		case TextureFormat::BGR8:
			typeOut = GL_UNSIGNED_BYTE;
			return GL_BGR;
		case TextureFormat::BGRA8:
			typeOut = GL_UNSIGNED_BYTE;
			return GL_BGRA;
		default:
			return GL_INVALID_ENUM;
	}
}
