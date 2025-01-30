#include "buffer.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API D3D11Buffer::D3D11Buffer(
	Device *ownerDevice,
	const BufferDesc &bufferDesc,
	ID3D11Buffer *buffer)
	: Buffer(ownerDevice, bufferDesc),
	  buffer(buffer) {
}

CLCGHAL_API D3D11Buffer::~D3D11Buffer() {
}
