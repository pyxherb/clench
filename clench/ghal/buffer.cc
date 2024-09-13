#include "buffer.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API Buffer::Buffer(GHALDevice *ownerDevice, const BufferDesc &bufferDesc)
	: GHALDeviceResource(ownerDevice),
	  bufferDesc(bufferDesc) {
}

CLCGHAL_API Buffer::~Buffer() {
}
