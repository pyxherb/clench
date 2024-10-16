#include <clench/modrt/modrt.h>
#include "basedefs.h"

namespace dftboot {
	bool moduleInit() {
		puts("Hello, world from dftboot");
		return true;
	}

	void moduleDeinit() {
	}

	static const char *dependencies[] = {
		"world3d"
	};

	DFTBOOT_API clench::mod::ModuleEntry moduleEntry("dftboot", std::size(dependencies), dependencies, moduleInit, moduleDeinit, true);
}
