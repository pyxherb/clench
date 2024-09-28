#include "decoder.h"
#include <clench/utils/assert.h>
#include <clench/utils/logger.h>

using namespace clench;
using namespace clench::tcm;

CLCTCM_API std::unordered_map<std::string, std::unique_ptr<Decoder>> clench::tcm::g_registeredDecoders;

CLCTCM_API Decoder::~Decoder() {}

CLCTCM_API void registerDecoder(const std::string &name, Decoder *decoder) {
	CLENCH_DEBUG_LOG("GHAL", "Registering GHAL backend: %s", name.data());
	CLENCH_ASSERT(!g_registeredDecoders.count(name), "GHAL backend with the same name already exists");
}

CLCTCM_API void unregisterDecoder(const std::string &name) {
	CLENCH_DEBUG_LOG("GHAL", "Unregistering TCM decoder: %s", name.c_str());

	auto it = g_registeredDecoders.find(name);

	CLENCH_ASSERT(it != g_registeredDecoders.end(), "No such TCM decoder");

	g_registeredDecoders.erase(it);
}
