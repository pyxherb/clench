#include "device.h"
#include "vertex_array.h"
#include "shader.h"
#include "buffer.h"
#include "views.h"
#include "texture.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLGHALDevice::GLGHALDevice(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, GLGHALBackend *backend)
	: GHALDevice(selfAllocator, resourceAllocator),
	  backend(backend) {
}

CLCGHAL_API GLGHALDevice::~GLGHALDevice() {
}

CLCGHAL_API GHALBackend *GLGHALDevice::getBackend() {
	return backend;
}

CLCGHAL_API GHALDeviceContext *GLGHALDevice::createDeviceContextForWindow(clench::wsal::NativeWindow *window) {
	int width, height;
	window->getSize(width, height);

	std::unique_ptr<
		GLGHALDeviceContext,
		peff::RcObjectUniquePtrDeleter>
		deviceContext(GLGHALDeviceContext::alloc(this));
	if (!deviceContext)
		return nullptr;
#ifdef _WIN32
	deviceContext->hWnd = window->nativeHandle;
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

	NativeGLContext prevContext = GLGHALDeviceContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() {
		GLGHALDeviceContext::restoreContextCurrent(prevContext);
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

	deviceContext->eglDisplay = eglGetDisplay((EGLNativeDisplayType)window->nativeHandle.data.x11.display);
	if (auto it = g_initializedEglDisplays.find(deviceContext->eglDisplay); it != g_initializedEglDisplays.end()) {
		++it->second;
	} else {
		eglInitialize(deviceContext->eglDisplay, &eglMajor, &eglMinor);
		g_initializedEglDisplays[deviceContext->eglDisplay] = 1;
	}

	deviceContext->eglWindow = (EGLNativeWindowType)window->nativeHandle.data.x11.windowId;

	EGLint nConfigs;
	eglChooseConfig(deviceContext->eglDisplay, configAttribs, &deviceContext->eglConfig, 1, &nConfigs);

	deviceContext->eglSurface = eglCreateWindowSurface(
		deviceContext->eglDisplay,
		deviceContext->eglConfig,
		deviceContext->eglWindow,
		nullptr);
	if (deviceContext->eglSurface == EGL_NO_SURFACE)
		throw std::runtime_error("Error creating EGL surface");

	eglBindAPI(EGL_OPENGL_API);

	static EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	deviceContext->eglContext = eglCreateContext(deviceContext->eglDisplay, deviceContext->eglConfig, EGL_NO_CONTEXT, contextAttribs);

	NativeGLContext prevContext = GLGHALDeviceContext::saveContextCurrent();
	peff::ScopeGuard restoreContextGuard([&prevContext]() {
		GLGHALDeviceContext::restoreContextCurrent(prevContext);
	});
	deviceContext->makeContextCurrent();

	if (!g_glInitialized) {
		int version = gladLoadGL((GLADloadfunc)_loadGlProc);
		if (!version)
			throw std::runtime_error("Error initializing GLAD");
		g_glInitialized = true;
	}
#endif
	GLint defaultFramebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&defaultFramebuffer);
	deviceContext->defaultRenderTargetView = GLRenderTargetView::alloc(this, RenderTargetViewType::Buffer, defaultFramebuffer);

	deviceContext->onResize(width, height);
	return deviceContext.release();
}

CLCGHAL_API VertexArray *GLGHALDevice::createVertexArray(
	VertexArrayElementDesc *elementDescs,
	size_t nElementDescs,
	VertexShader *vertexShader) {
	GLuint vao;
	glGenVertexArrays(1, &vao);

	peff::ScopeGuard deleteVaoGuard([&vao]() {
		glDeleteVertexArrays(1, &vao);
	});

	GLint prevVao;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVao);
	peff::ScopeGuard restoreTextureGuard([prevVao]() {
		glBindVertexArray(prevVao);
	});

	std::unique_ptr<GLVertexArray, peff::RcObjectUniquePtrDeleter> vertexArray(GLVertexArray::alloc(this, vao));

	for (size_t i = 0; i < nElementDescs; ++i) {
		VertexArrayElementDesc &curDesc = elementDescs[i];

		glEnableVertexArrayAttrib(vao, i);

		size_t sizeOut;
		GLenum glType;
		if((glType = toGLVertexDataType(curDesc.dataType, sizeOut)) == GL_INVALID_ENUM) {
			return nullptr;
		}

		glVertexAttribPointer(i, sizeOut * curDesc.dataType.nElements, glType, false, curDesc.stride, (void *)curDesc.off);
	}

	deleteVaoGuard.release();

	return vertexArray.release();
}

CLCGHAL_API bool GLGHALDevice::isVertexDataTypeSupported(const VertexDataType &vertexDataType) {
}

CLCGHAL_API VertexShader *GLGHALDevice::createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) {
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	peff::ScopeGuard deleteShaderGuard([shader]() {
		glDeleteShader(shader);
	});

	glShaderSource(shader, 1, &source, (GLint *)&size);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[1024 + 1];
		log[1024] = '\0';
		GLsizei size = 1024;
		glGetShaderInfoLog(shader, 1024, &size, log);
		throw std::runtime_error("Error compiling shader: " + std::string(log));
	}

	std::unique_ptr<GLVertexShader, peff::RcObjectUniquePtrDeleter> vertexShader(GLVertexShader::alloc(this, shader));
	if (!vertexShader)
		return nullptr;

	deleteShaderGuard.release();

	return vertexShader.release();
}

CLCGHAL_API FragmentShader *GLGHALDevice::createFragmentShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) {
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	peff::ScopeGuard deleteShaderGuard([shader]() {
		glDeleteShader(shader);
	});

	glShaderSource(shader, 1, &source, (GLint *)&size);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[1024 + 1];
		log[1024] = '\0';
		GLsizei size = 1024;
		glGetShaderInfoLog(shader, 1024, &size, log);
		throw std::runtime_error("Error compiling shader: " + std::string(log));
	}

	std::unique_ptr<GLFragmentShader, peff::RcObjectUniquePtrDeleter> fragmentShader(GLFragmentShader::alloc(this, shader));
	if (!fragmentShader)
		return nullptr;

	deleteShaderGuard.release();

	return fragmentShader.release();
}

CLCGHAL_API GeometryShader *GLGHALDevice::createGeometryShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) {
	return nullptr;
}

CLCGHAL_API ShaderProgram *GLGHALDevice::linkShaderProgram(Shader **shaders, size_t nShaders) {
	GLVertexShader *vertexShader = nullptr;
	GLFragmentShader *fragmentShader = nullptr;

	for (size_t i = 0; i < nShaders; ++i) {
		Shader *curShader = shaders[i];

		switch (curShader->shaderType) {
			case ShaderType::Vertex: {
				if (vertexShader) {
					throw std::runtime_error("Duplicated vertex shader");
				}
				vertexShader = (GLVertexShader *)curShader;
				break;
			}
			case ShaderType::Fragment: {
				if (fragmentShader) {
					throw std::runtime_error("Duplicated fragment shader");
				}
				fragmentShader = (GLFragmentShader *)curShader;
				break;
			}
			default:
				break;
		}
	}

	if (!vertexShader)
		throw std::logic_error("Missing vertex shader");

	if (!fragmentShader)
		throw std::logic_error("Missing fragment shader");

	GLuint program = glCreateProgram();
	peff::ScopeGuard deleteProgramGuard([program]() {
		glDeleteProgram(program);
	});

	glAttachShader(program, vertexShader->shaderHandle);
	glAttachShader(program, fragmentShader->shaderHandle);

	GLint success;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char log[1024 + 1];
		log[1024] = '\0';
		GLsizei size = 1024;
		glGetProgramInfoLog(program, 1024, &size, log);
		throw std::runtime_error("Error linking shaders: " + std::string(log));
	}

	std::unique_ptr<GLShaderProgram, peff::RcObjectUniquePtrDeleter> shaderProgram(GLShaderProgram::alloc(this, program));
	if (!shaderProgram)
		return nullptr;

	deleteProgramGuard.release();

	return shaderProgram.release();
}

CLCGHAL_API Buffer *GLGHALDevice::createBuffer(const BufferDesc &bufferDesc, const void *initialData) {
	GLuint buffer;
	glGenBuffers(1, &buffer);
	peff::ScopeGuard deleteBufferGuard([buffer]() {
		glDeleteBuffers(1, &buffer);
	});

	std::unique_ptr<GLBuffer, peff::RcObjectUniquePtrDeleter> glBuffer(GLBuffer::alloc(this, bufferDesc, buffer));

	deleteBufferGuard.release();

	defaultContext->setData(glBuffer.get(), initialData);

	return glBuffer.release();
}

CLCGHAL_API Texture1D *GLGHALDevice::createTexture1D(const char *data, size_t size, const Texture1DDesc &desc) {
	GLenum glType;
	GLenum glTextureFormat = toGLTextureFormat(desc.format, glType);
	if (glTextureFormat == GL_INVALID_ENUM)
		throw std::runtime_error("Invalid texture format");

	GLuint texture;
	glGenTextures(1, &texture);
	peff::ScopeGuard deleteTextureGuard([texture]() {
		glDeleteTextures(1, &texture);
	});

	texture1dLock.lock();
	GLint savedTexture;
	glGetIntegerv(GL_TEXTURE_1D, &savedTexture);
	peff::ScopeGuard restoreTextureGuard([this, savedTexture]() {
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

	GLTexture1D *texture1d = new GLTexture1D(this, desc, texture);

	deleteTextureGuard.release();

	return texture1d;
}

CLCGHAL_API Texture2D *GLGHALDevice::createTexture2D(const char *data, size_t size, const Texture2DDesc &desc) {
	GLenum glType;
	GLenum glTextureFormat = toGLTextureFormat(desc.format, glType);
	if (glTextureFormat == GL_INVALID_ENUM)
		throw std::runtime_error("Invalid texture format");

	GLuint texture;
	glGenTextures(1, &texture);
	peff::ScopeGuard deleteTextureGuard([texture]() {
		glDeleteTextures(1, &texture);
	});

	texture2dLock.lock();
	GLint savedTexture;
	glGetIntegerv(GL_TEXTURE_2D, &savedTexture);
	peff::ScopeGuard restoreTextureGuard([this, savedTexture]() {
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

	GLTexture2D *texture2d = new GLTexture2D(this, desc, texture);

	deleteTextureGuard.release();

	return texture2d;
}

CLCGHAL_API Texture3D *GLGHALDevice::createTexture3D(const char *data, size_t size, const Texture3DDesc &desc) {
	GLenum glType;
	GLenum glTextureFormat = toGLTextureFormat(desc.format, glType);
	if (glTextureFormat == GL_INVALID_ENUM)
		throw std::runtime_error("Invalid texture format");

	GLuint texture;
	glGenTextures(1, &texture);
	peff::ScopeGuard deleteTextureGuard([texture]() {
		glDeleteTextures(1, &texture);
	});

	texture3dLock.lock();
	GLuint savedTexture;
	glGetIntegerv(GL_TEXTURE_3D, (GLint *)&savedTexture);
	peff::ScopeGuard restoreTextureGuard([this, savedTexture]() {
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

	GLTexture3D *texture3d = new GLTexture3D(this, desc, texture);

	deleteTextureGuard.release();

	return texture3d;
}

CLCGHAL_API RenderTargetView *GLGHALDevice::createRenderTargetViewForTexture2D(Texture2D *texture) {
	return GLRenderTargetView::alloc(this, RenderTargetViewType::Texture2D, ((GLTexture2D *)texture)->textureHandle);
}

CLCGHAL_API GLGHALDevice *GLGHALDevice::alloc(peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, GLGHALBackend *backend) {
	std::unique_ptr<GLGHALDevice, peff::DeallocableDeleter> ptr(
		peff::allocAndConstruct<GLGHALDevice>(
			selfAllocator, sizeof(std::max_align_t),
			selfAllocator, resourceAllocator, backend));
	if (!ptr)
		return nullptr;

	if (!(ptr->defaultContext = GLGHALDeviceContext::alloc(ptr.get())))
		return nullptr;

	return (GLGHALDevice *)ptr.release();
}

CLCGHAL_API GLGHALDeviceContext::GLGHALDeviceContext(
	GLGHALDevice *device)
	: GHALDeviceContext(device) {
}

CLCGHAL_API GLGHALDeviceContext::~GLGHALDeviceContext() {
#if _WIN32
	if (wglContext)
		wglDeleteContext(wglContext);
	if (hdc)
		ReleaseDC(hWnd, hdc);
#else
	if (eglContext != EGL_NO_CONTEXT) {
		eglDestroyContext(eglDisplay, eglContext);
	}

	if (eglSurface != EGL_NO_SURFACE) {
		eglDestroySurface(eglDisplay, eglSurface);
	}

	if (eglDisplay != EGL_NO_DISPLAY) {
		if (auto it = g_initializedEglDisplays.find(eglDisplay); it != g_initializedEglDisplays.end()) {
			if (!--it->second) {
				eglTerminate(eglDisplay);
				g_initializedEglDisplays.erase(it);
			}
		} else {
			eglTerminate(eglDisplay);
		}
	}
#endif
}

CLCGHAL_API RenderTargetView *GLGHALDeviceContext::getDefaultRenderTargetView() {
	return defaultRenderTargetView.get();
}

CLCGHAL_API void GLGHALDeviceContext::onResize(int width, int height) {
#ifdef _WIN32
#else
	saveCurrentGLContext();

	windowWidth = width;
	windowHeight = height;

	if (eglSurface != EGL_NO_SURFACE)
		eglDestroySurface(eglDisplay, eglSurface);

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, nullptr);
	if (eglSurface == EGL_NO_SURFACE)
		throw std::runtime_error("Error creating EGL surface");

	restoreCurrentGLContext();
#endif
}

CLCGHAL_API void GLGHALDeviceContext::clearRenderTargetView(
	RenderTargetView *renderTargetView,
	float r,
	float g,
	float b,
	float a) {
	saveCurrentGLContext();

	GLuint prevRenderTarget;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&prevRenderTarget);

	peff::ScopeGuard restoreRenderTargetGuard([prevRenderTarget]() {
		glBindFramebuffer(GL_FRAMEBUFFER, prevRenderTarget);
	});

	if (!renderTargetView)
		renderTargetView = defaultRenderTargetView.get();

	glBindFramebuffer(GL_FRAMEBUFFER, ((GLRenderTargetView *)renderTargetView)->frameBufferHandle);
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);

	restoreCurrentGLContext();
}

CLCGHAL_API void GLGHALDeviceContext::clearDepth(
	DepthStencilView *depthStencilView,
	float depth) {
	saveCurrentGLContext();

	GLuint prevRenderTarget;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&prevRenderTarget);

	peff::ScopeGuard restoreRenderTargetGuard([prevRenderTarget]() {
		glBindFramebuffer(GL_FRAMEBUFFER, prevRenderTarget);
	});

	glBindFramebuffer(GL_FRAMEBUFFER, ((GLDepthStencilView *)depthStencilView)->frameBufferHandle);
	glDepthMask(GL_TRUE);
	glClearDepth(depth);
	glClear(GL_DEPTH_BUFFER_BIT);

	restoreCurrentGLContext();
}

CLCGHAL_API void GLGHALDeviceContext::clearStencil(
	DepthStencilView *depthStencilView,
	uint8_t stencil) {
	saveCurrentGLContext();

	GLuint prevRenderTarget;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&prevRenderTarget);

	peff::ScopeGuard restoreRenderTargetGuard([prevRenderTarget]() {
		glBindFramebuffer(GL_FRAMEBUFFER, prevRenderTarget);
	});

	glBindFramebuffer(GL_FRAMEBUFFER, ((GLDepthStencilView *)depthStencilView)->frameBufferHandle);
	glStencilMask(0xff);
	glClearStencil(stencil);
	glClear(GL_STENCIL_BUFFER_BIT);

	restoreCurrentGLContext();
}

CLCGHAL_API void GLGHALDeviceContext::bindVertexBuffer(Buffer *buffer, size_t stride) {
	saveCurrentGLContext();

	glBindBuffer(GL_ARRAY_BUFFER, ((GLBuffer *)buffer)->bufferHandle);

	restoreCurrentGLContext();
}

CLCGHAL_API void GLGHALDeviceContext::bindIndexBuffer(Buffer *buffer) {
	saveCurrentGLContext();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLBuffer *)buffer)->bufferHandle);

	restoreCurrentGLContext();
}

CLCGHAL_API void GLGHALDeviceContext::bindVertexArray(VertexArray *vertexArray) {
	saveCurrentGLContext();

	glBindVertexArray(((GLVertexArray *)vertexArray)->vertexArrayHandle);

	restoreCurrentGLContext();
}

CLCGHAL_API void GLGHALDeviceContext::setData(Buffer *buffer, const void *data) {
	copyWriteBufferLock.lock();
	GLuint prevBuffer;
	glGetIntegerv(GL_COPY_WRITE_BUFFER, (GLint *)&prevBuffer);
	peff::ScopeGuard restoreBufferGuard([this, prevBuffer]() {
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

CLCGHAL_API void GLGHALDeviceContext::setShaderProgram(ShaderProgram *shaderProgram) {
	glUseProgram(((GLShaderProgram *)shaderProgram)->programHandle);
}

CLCGHAL_API void GLGHALDeviceContext::setRenderTarget(
	RenderTargetView *renderTargetView,
	DepthStencilView *depthStencilView) {
	glBindFramebuffer(GL_FRAMEBUFFER, ((GLRenderTargetView *)renderTargetView)->frameBufferHandle);

	renderBufferLock.lock();
	GLuint prevRenderTarget;
	glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint *)&prevRenderTarget);

	peff::ScopeGuard restoreRenderTargetGuard([this, prevRenderTarget]() {
		glBindRenderbuffer(GL_RENDERBUFFER, prevRenderTarget);
		renderBufferLock.unlock();
	});

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ((GLDepthStencilView *)depthStencilView)->frameBufferHandle);
}

CLCGHAL_API void GLGHALDeviceContext::setViewport(
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

CLCGHAL_API void GLGHALDeviceContext::getViewport(
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

CLCGHAL_API void GLGHALDeviceContext::drawIndexed(unsigned int nIndices) {
	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, nullptr);
}

CLCGHAL_API void GLGHALDeviceContext::begin() {
	makeContextCurrent();
}

CLCGHAL_API void GLGHALDeviceContext::end() {
}

CLCGHAL_API void GLGHALDeviceContext::present() {
#ifdef _WIN32
	SwapBuffers(hdc);
#else
	eglSwapBuffers(eglDisplay, eglSurface);
#endif
}

CLCGHAL_API NativeGLContext GLGHALDeviceContext::saveContextCurrent() {
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

CLCGHAL_API bool GLGHALDeviceContext::restoreContextCurrent(const NativeGLContext &context) {
#if _WIN32
	return wglMakeCurrent(context.hdc, context.wglContext);
#else
	return eglMakeCurrent(context.eglDisplay, context.eglDrawSurface, context.eglReadSurface, context.eglContext);
#endif
}

CLCGHAL_API void GLGHALDeviceContext::saveCurrentGLContext() {
	prevContextSavingMutex.lock();
	prevContext = saveContextCurrent();
	makeContextCurrent();
}

CLCGHAL_API void GLGHALDeviceContext::restoreCurrentGLContext() {
	restoreContextCurrent(prevContext.value());
	prevContext.reset();
	prevContextSavingMutex.unlock();
}

CLCGHAL_API bool GLGHALDeviceContext::makeContextCurrent() {
#if _WIN32
	return wglMakeCurrent(hdc, wglContext);
#else
	return eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
#endif
}

CLCGHAL_API GLGHALDeviceContext *GLGHALDeviceContext::alloc(GLGHALDevice *device) {
	return peff::allocAndConstruct<GLGHALDeviceContext>(
		device->resourceAllocator.get(), sizeof(std::max_align_t),
		device);
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
