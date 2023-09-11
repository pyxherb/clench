#include "../context.h"

#ifdef _WIN32
	#include <Windows.h>
#endif

using namespace clench::graphics;

#ifdef _WIN32
HMODULE clench::graphics::hOpenGL32Dll;
#endif

GLBackend::GLBackend() : GraphicsBackend("opengl") {}

GLBackend::~GLBackend() {}

void GLBackend::init() {
	_initGL();
}

void GLBackend::deinit() {
	_deinitGL();
}

#ifdef _WIN32
void GLBackend::_initGL() {
	if (!(hOpenGL32Dll = LoadLibrary("opengl32.dll")))
		throw std::runtime_error("Error loading OpenGL DLL");
}

void GLBackend::_deinitGL() {
	if (hOpenGL32Dll)
		FreeLibrary(hOpenGL32Dll);
}
#else
void GLBackend::_initGL() {
}

void GLBackend::_deinitGL() {
}
#endif

GLBackend backend;
