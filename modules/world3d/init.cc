#include <clench/modrt/modrt.h>

static bool moduleInit() {
	puts("Hello, world from world3d");
	return true;
}

static void moduleDeinit() {
}

static clench::mod::ModuleEntry moduleEntry("world3d", 0, nullptr, moduleInit, moduleDeinit);
