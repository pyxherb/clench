#include "buffer.h"

using namespace clench;
using namespace clench::ahal;

CLCAHAL_API Buffer::Buffer(Device *ownerDevice, const BufferDesc &bufferDesc)
	: DeviceResource(ownerDevice),
	  bufferDesc(bufferDesc) {
}

CLCAHAL_API Buffer::~Buffer() {
}
