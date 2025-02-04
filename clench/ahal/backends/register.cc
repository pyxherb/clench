#include "../backend.h"

#if CLCAHAL_HAS_ALSA_BACKEND
	#include "alsa/backend.h"
#endif

using namespace clench;
using namespace clench::ahal;

CLCAHAL_API void clench::ahal::registerBuiltinBackends(peff::Alloc *selfAllocator) {
#if CLCAHAL_HAS_ALSA_BACKEND
	registerBackend(ALSABackend::alloc(selfAllocator));
#endif
}
