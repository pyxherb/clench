#include "init.h"
#include <clench/modrt/modrt.h>

using namespace world3d;

WORLD3D_API bool world3d::g_isInited = false;

static bool _moduleInit() {
	world3d::g_isInited = true;
	puts("Hello, world from world3d");
	return true;
}

static void _moduleDeinit() {
	world3d::g_isInited = false;
}

static clench::mod::ModuleEntry moduleEntry("world3d", 0, nullptr, _moduleInit, _moduleDeinit, true);
