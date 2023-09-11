#include "../context.h"
#ifdef _WIN32
	#include <clench/window/win32.h>
#else
	#include <clench/window/x11.h>
	#include <EGL/egl.h>
#endif

using namespace clench;
using namespace clench::graphics;

static void* _loadGlProc(const char* name);

static void _glDebugMessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

GLContext::GLContext(Window* window) : _window(window) {
#ifdef _WIN32
	_hdc = GetDC((HWND)_window->getNativeHandle());
	{
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;

		auto pxFmt = ChoosePixelFormat(_hdc, &pfd);
		if (!pxFmt)
			throw std::runtime_error("Incompatible graphics device");

		assert(SetPixelFormat(_hdc, pxFmt, &pfd));
	}

	if (!(_wglContext = wglCreateContext(_hdc)))
		throw std::runtime_error("Error creating WGL context");

	if (!wglMakeCurrent(_hdc, _wglContext))
		throw std::runtime_error("Error setting current WGL context");
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

	auto windowType = window->getBackend().getName();
	if (windowType == "X11") {
		_eglDisplay = eglGetDisplay((EGLNativeDisplayType)((X11Window*)window)->display);
		_eglWindow = *(EGLNativeWindowType*)window->getNativeHandle();
	} else
		throw std::runtime_error("Unrecognized window backend: " + windowType);

	eglInitialize(_eglDisplay, &_eglMajor, &_eglMinor);

	EGLint nConfigs;
	eglChooseConfig(_eglDisplay, configAttribs, &_eglConfig, 1, &nConfigs);

	_eglSurface = eglCreateWindowSurface(_eglDisplay, _eglConfig, _eglWindow, nullptr);
	if (_eglSurface == EGL_NO_SURFACE)
		throw std::runtime_error("Error creating EGL surface");

	eglBindAPI(EGL_OPENGL_API);

	static EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	_eglContext = eglCreateContext(_eglDisplay, _eglConfig, EGL_NO_CONTEXT, contextAttribs);
	eglMakeCurrent(_eglDisplay, _eglSurface, _eglSurface, _eglContext);
#endif
	if (!gladLoadGLLoader((GLADloadproc)_loadGlProc))
		throw std::runtime_error("Error initializing OpenGL");

	debug::info("OpenGL version: %s", glGetString(GL_VERSION));

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(_glDebugMessageCallback, nullptr);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLContext::~GLContext() {
#ifdef _DEBUG
	// Check if all resources were released
	for (auto& i : _bakedResources)
		assert(i.expired());
#endif

#ifdef _WIN32
	wglDeleteContext(_wglContext);
	ReleaseDC((HWND)_window->getNativeHandle(), _hdc);
#else
	eglTerminate(_eglDisplay);
#endif
}

clench::graphics::GraphicsContext* GLBackend::createGraphicsContext(Window* window) {
	return new GLContext(window);
}

static void* _loadGlProc(const char* name) {
#ifdef _WIN32
	void* addr = wglGetProcAddress(name);

	if (addr)
		return addr;

	return GetProcAddress(hOpenGL32Dll, name);
#else
	return (void*)eglGetProcAddress(name);
#endif
}

static void _glDebugMessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {
	std::string src = "Unknown", typeStr = "Unknown";
	decltype(debug::info)* logger = debug::info;

	switch (source) {
		case GL_DEBUG_SOURCE_API:
			src = "API";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			src = "Application";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			src = "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			src = "3rd Party";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			src = "Window System";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			src = "Other";
			break;
	}

	switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			typeStr = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeStr = "Deprecated";
			break;
		case GL_DEBUG_TYPE_MARKER:
			typeStr = "Marker";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typeStr = "Performance";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			typeStr = "Pop Group";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			typeStr = "Push Group";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typeStr = "Portability";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeStr = "Undefined Behavior";
			break;
		case GL_DEBUG_TYPE_OTHER:
			typeStr = "Other";
		default:
			break;
	}

	switch (severity) {
		case GL_DEBUG_SEVERITY_LOW:
			logger = debug::note;
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			logger = debug::warn;
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			logger = debug::error;
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			logger = debug::info;
			break;
	}

	logger("[OPENGL/%s] %s: %s", typeStr.c_str(), src.c_str(), std::string(message, length).c_str());

	// assert(severity != GL_DEBUG_SEVERITY_HIGH);
}
