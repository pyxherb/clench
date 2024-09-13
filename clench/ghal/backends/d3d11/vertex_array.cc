#include "vertex_array.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API D3D11VertexArray::D3D11VertexArray(
	GHALDevice *ownerDevice,
	ID3D11InputLayout *inputLayout)
	: VertexArray(ownerDevice),
	  inputLayout(inputLayout) {
}

CLCGHAL_API D3D11VertexArray::~D3D11VertexArray() {
}
