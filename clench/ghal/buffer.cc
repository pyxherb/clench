#include "buffer.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API Buffer::Buffer(Device *ownerDevice, const BufferDesc &bufferDesc)
	: DeviceResource(ownerDevice),
	  bufferDesc(bufferDesc) {
}

CLCGHAL_API Buffer::~Buffer() {
}
