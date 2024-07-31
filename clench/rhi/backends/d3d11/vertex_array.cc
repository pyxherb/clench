#include "vertex_array.h"

using namespace clench;
using namespace clench::rhi;

CLCRHI_API D3D11VertexArray::D3D11VertexArray(
	RHIDevice *ownerDevice,
	ID3D11InputLayout *inputLayout)
	: VertexArray(ownerDevice),
	  inputLayout(inputLayout) {
}

CLCRHI_API D3D11VertexArray::~D3D11VertexArray() {
}
