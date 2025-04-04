#include "vertex_layout.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API D3D11VertexLayout::D3D11VertexLayout(
	Device *ownerDevice,
	ID3D11InputLayout *inputLayout)
	: VertexLayout(ownerDevice),
	  inputLayout(inputLayout) {
}

CLCGHAL_API D3D11VertexLayout::~D3D11VertexLayout() {
}
