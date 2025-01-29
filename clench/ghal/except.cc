#include "except.h"

using namespace clench;
using namespace clench::ghal;

#pragma region GHALException
CLCGHAL_API GHALException::GHALException(peff::Alloc *allocator, GHALExceptionCode ghalExceptionCode) : Exception(allocator, EXCEPTION_TYPE_GHAL), ghalExceptionCode(ghalExceptionCode) {
}

CLCGHAL_API GHALException::~GHALException() {
}
#pragma endregion

#pragma region ErrorCreatingDeviceContextException
CLCGHAL_API ErrorCreatingDeviceContextException::ErrorCreatingDeviceContextException(peff::Alloc *allocator, base::ExceptionPtr &&minorException) : GHALException(allocator, GHALExceptionCode::ErrorCreatingDeviceContext), minorException(std::move(minorException)) {
}

CLCGHAL_API ErrorCreatingDeviceContextException::~ErrorCreatingDeviceContextException() {
}

CLCGHAL_API const char *ErrorCreatingDeviceContextException::what() const {
	return "Error creating the GHAL device context";
}

CLCGHAL_API void ErrorCreatingDeviceContextException::dealloc() {
	peff::destroyAndRelease<ErrorCreatingDeviceContextException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API ErrorCreatingDeviceContextException *ErrorCreatingDeviceContextException::alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException) {
	return peff::allocAndConstruct<ErrorCreatingDeviceContextException>(allocator, sizeof(std::max_align_t), allocator, std::move(minorException));
}
#pragma endregion

#pragma region ErrorCreatingResourceException
CLCGHAL_API ErrorCreatingResourceException::ErrorCreatingResourceException(peff::Alloc *allocator, base::ExceptionPtr &&minorException) : GHALException(allocator, GHALExceptionCode::ErrorCreatingResource), minorException(std::move(minorException)) {
}

CLCGHAL_API ErrorCreatingResourceException::~ErrorCreatingResourceException() {
}

CLCGHAL_API const char *ErrorCreatingResourceException::what() const {
	return "Error creating the GHAL resource";
}

CLCGHAL_API void ErrorCreatingResourceException::dealloc() {
	peff::destroyAndRelease<ErrorCreatingResourceException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API ErrorCreatingResourceException *ErrorCreatingResourceException::alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException) {
	return peff::allocAndConstruct<ErrorCreatingResourceException>(allocator, sizeof(std::max_align_t), allocator, std::move(minorException));
}
#pragma endregion

#pragma region InvalidVertexDataTypeException
CLCGHAL_API InvalidVertexDataTypeException::InvalidVertexDataTypeException(peff::Alloc *allocator, size_t idxDesc) : GHALException(allocator, GHALExceptionCode::InvalidVertexDataType), idxDesc(idxDesc) {
}

CLCGHAL_API InvalidVertexDataTypeException::~InvalidVertexDataTypeException() {
}

CLCGHAL_API const char *InvalidVertexDataTypeException::what() const {
	return "Invalid vertex data type for vertex layout";
}

CLCGHAL_API void InvalidVertexDataTypeException::dealloc() {
	peff::destroyAndRelease<InvalidVertexDataTypeException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API InvalidVertexDataTypeException *InvalidVertexDataTypeException::alloc(peff::Alloc *allocator, size_t idxDesc) {
	return peff::allocAndConstruct<InvalidVertexDataTypeException>(allocator, sizeof(std::max_align_t), allocator, idxDesc);
}
#pragma endregion

#pragma region ErrorCompilingShaderException
CLCGHAL_API ErrorCompilingShaderException::ErrorCompilingShaderException(
	peff::Alloc *allocator,
	peff::String &&message) : GHALException(allocator, GHALExceptionCode::ErrorCompilingShader), message(std::move(message)) {
}

CLCGHAL_API ErrorCompilingShaderException::~ErrorCompilingShaderException() {
}

CLCGHAL_API const char *ErrorCompilingShaderException::what() const {
	return "Error compiling the shader";
}

CLCGHAL_API void ErrorCompilingShaderException::dealloc() {
	peff::destroyAndRelease<ErrorCompilingShaderException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API ErrorCompilingShaderException *ErrorCompilingShaderException::alloc(
	peff::Alloc *allocator,
	peff::String &&message) {
	return peff::allocAndConstruct<ErrorCompilingShaderException>(allocator, sizeof(std::max_align_t), allocator, std::move(message));
}
#pragma endregion

#pragma region ErrorLinkingShaderException
CLCGHAL_API ErrorLinkingShaderException::ErrorLinkingShaderException(
	peff::Alloc *allocator,
	peff::String &&message) : GHALException(allocator, GHALExceptionCode::ErrorLinkingShader), message(std::move(message)) {
}

CLCGHAL_API ErrorLinkingShaderException::~ErrorLinkingShaderException() {
}

CLCGHAL_API const char *ErrorLinkingShaderException::what() const {
	return "Error linking the shaders";
}

CLCGHAL_API void ErrorLinkingShaderException::dealloc() {
	peff::destroyAndRelease<ErrorLinkingShaderException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API ErrorLinkingShaderException *ErrorLinkingShaderException::alloc(
	peff::Alloc *allocator,
	peff::String &&message) {
	return peff::allocAndConstruct<ErrorLinkingShaderException>(allocator, sizeof(std::max_align_t), allocator, std::move(message));
}
#pragma endregion

#pragma region MissingShaderPartException
CLCGHAL_API MissingShaderPartException::MissingShaderPartException(
	peff::Alloc *allocator,
	ShaderType shaderType) : GHALException(allocator, GHALExceptionCode::MissingShaderPart), shaderType(shaderType) {
}

CLCGHAL_API MissingShaderPartException::~MissingShaderPartException() {
}

CLCGHAL_API const char *MissingShaderPartException::what() const {
	return "Error compiling the shader";
}

CLCGHAL_API void MissingShaderPartException::dealloc() {
	peff::destroyAndRelease<MissingShaderPartException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API MissingShaderPartException *MissingShaderPartException::alloc(
	peff::Alloc *allocator,
	ShaderType shaderType) {
	return peff::allocAndConstruct<MissingShaderPartException>(allocator, sizeof(std::max_align_t), allocator, shaderType);
}
#pragma endregion

#pragma region DuplicatedShaderPartException
CLCGHAL_API DuplicatedShaderPartException::DuplicatedShaderPartException(peff::Alloc *allocator, size_t idxShader) : GHALException(allocator, GHALExceptionCode::DuplicatedShaderPart), idxShader(idxShader) {
}

CLCGHAL_API DuplicatedShaderPartException::~DuplicatedShaderPartException() {
}

CLCGHAL_API const char *DuplicatedShaderPartException::what() const {
	return "Invalid vertex data type for vertex layout";
}

CLCGHAL_API void DuplicatedShaderPartException::dealloc() {
	peff::destroyAndRelease<DuplicatedShaderPartException>(allocator.get(), this, sizeof(std::max_align_t));
}

CLCGHAL_API DuplicatedShaderPartException *DuplicatedShaderPartException::alloc(peff::Alloc *allocator, size_t idxShader) {
	return peff::allocAndConstruct<DuplicatedShaderPartException>(allocator, sizeof(std::max_align_t), allocator, idxShader);
}
#pragma endregion

#pragma region PlatformSpecificException
CLCGHAL_API PlatformSpecificException::PlatformSpecificException(peff::Alloc *allocator) : GHALException(allocator, GHALExceptionCode::PlatformSpecific) {
}

CLCGHAL_API PlatformSpecificException::~PlatformSpecificException() {
}
#pragma endregion
