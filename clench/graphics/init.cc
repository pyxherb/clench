#include "init.h"
#include <clench/debug.h>

void clench::graphics::init() {
	clench::debug::info("Initializing graphics system...");

	initBackends();
	initImageDecoders();

	clench::debug::info("Initialized graphics system...");
}

void clench::graphics::deinit() {
	clench::debug::info("Deinitializing graphics system...");

	deinitImageDecoders();
	deinitBackends();

	clench::debug::info("Deinitialized graphics system...");
}
