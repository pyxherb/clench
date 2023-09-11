#include "clench.h"

void clench::init(InitFlags flags) {
	clench::debug::info("Initializing Clench...");
	input::init();
	graphics::init();
	clench::debug::info("Initialized Clench");
}

void clench::deinit() {
	clench::debug::info("Deinitializing Clench...");
	graphics::deinit();
	input::deinit();
	clench::debug::info("Deinitialized Clench");
}
