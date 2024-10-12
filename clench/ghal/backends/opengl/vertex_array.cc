#include "vertex_array.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API GLVertexArray::GLVertexArray(
	GHALDevice *ownerDevice,
	GLuint vertexArrayHandle)
	: VertexArray(ownerDevice),
	  vertexArrayHandle(vertexArrayHandle) {
}

CLCGHAL_API GLVertexArray::~GLVertexArray() {
	glDeleteVertexArrays(1, &vertexArrayHandle);
}
