#include "../decoder.h"

#if CLCTCM_HAS_PNG_DECODER
	#include "png/decoder.h"
#endif

using namespace clench;
using namespace clench::tcm;

CLCTCM_API void clench::tcm::registerBuiltinDecoders() {
#if CLCTCM_HAS_PNG_DECODER
	registerDecoder("png", new PNGDecoder());
#endif
}
