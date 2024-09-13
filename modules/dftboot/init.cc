#include <clench/modrt/modrt.h>

static bool moduleInit() {
	puts("Hello, world from dftboot");
	return true;
}

static void moduleDeinit() {
}

static const char *dependencies[] = {
	"world3d"
};

static clench::mod::ModuleEntry moduleEntry("dftboot", std::size(dependencies), dependencies, moduleInit, moduleDeinit, true);
