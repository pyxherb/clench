#ifndef _CLENCH_GHAL_EXCEPT_H_
#define _CLENCH_GHAL_EXCEPT_H_

#include "basedefs.h"
#include "shader.h"
#include <peff/containers/string.h>
#include <clench/base/except.h>

namespace clench {
	namespace ghal {
		constexpr base::UUID
			EXCEPTION_TYPE_GHAL = CLENCH_UUID(0aac8fa6, 3b34, 47c4, 8f6e, 7aae71666d88);

		enum class GHALExceptionCode : uint32_t {
			ErrorCreatingDeviceContext = 0,
			ErrorCreatingResource,
			InvalidContext,
			InvalidArgs,
			InvalidVertexDataType,
			ErrorCompilingShader,
			MissingShaderPart,
			DuplicatedShaderPart,
			ErrorLinkingShader,
			PlatformSpecific
		};

		class GHALException : public base::Exception {
		public:
			GHALExceptionCode ghalExceptionCode;

			CLCGHAL_API GHALException(peff::Alloc *allocator, GHALExceptionCode ghalExceptionCode);
			CLCGHAL_API virtual ~GHALException();
		};

		class ErrorCreatingDeviceContextException : public GHALException {
		public:
			base::ExceptionPtr minorException;

			CLCGHAL_API ErrorCreatingDeviceContextException(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
			CLCGHAL_API virtual ~ErrorCreatingDeviceContextException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static ErrorCreatingDeviceContextException *alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
		};

		class ErrorCreatingResourceException : public GHALException {
		public:
			base::ExceptionPtr minorException;

			CLCGHAL_API ErrorCreatingResourceException(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
			CLCGHAL_API virtual ~ErrorCreatingResourceException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static ErrorCreatingResourceException *alloc(peff::Alloc *allocator, base::ExceptionPtr &&minorException);
		};

		class InvalidVertexDataTypeException : public GHALException {
		public:
			size_t idxDesc;

			CLCGHAL_API InvalidVertexDataTypeException(peff::Alloc *allocator, size_t idxDesc);
			CLCGHAL_API virtual ~InvalidVertexDataTypeException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static InvalidVertexDataTypeException *alloc(peff::Alloc *allocator, size_t idxDesc);
		};

		class ErrorCompilingShaderException : public GHALException {
		public:
			peff::String message;

			CLCGHAL_API ErrorCompilingShaderException(peff::Alloc *allocator, peff::String &&message);
			CLCGHAL_API virtual ~ErrorCompilingShaderException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static ErrorCompilingShaderException *alloc(peff::Alloc *allocator, peff::String &&message);
		};

		class ErrorLinkingShaderException : public GHALException {
		public:
			peff::String message;

			CLCGHAL_API ErrorLinkingShaderException(peff::Alloc *allocator, peff::String &&message);
			CLCGHAL_API virtual ~ErrorLinkingShaderException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static ErrorLinkingShaderException *alloc(peff::Alloc *allocator, peff::String &&message);
		};

		class MissingShaderPartException : public GHALException {
		public:
			ShaderType shaderType;

			CLCGHAL_API MissingShaderPartException(peff::Alloc *allocator, ShaderType shaderType);
			CLCGHAL_API virtual ~MissingShaderPartException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static MissingShaderPartException *alloc(peff::Alloc *allocator, ShaderType shaderType);
		};

		class DuplicatedShaderPartException : public GHALException {
		public:
			size_t idxShader;

			CLCGHAL_API DuplicatedShaderPartException(peff::Alloc *allocator, size_t idxShader);
			CLCGHAL_API virtual ~DuplicatedShaderPartException();

			CLCGHAL_API const char *what() const override;
			CLCGHAL_API void dealloc() override;

			CLCGHAL_API static DuplicatedShaderPartException *alloc(peff::Alloc *allocator, size_t idxShader);
		};

		class PlatformSpecificException : public GHALException {
		public:
			CLCGHAL_API PlatformSpecificException(peff::Alloc *allocator);
			CLCGHAL_API virtual ~PlatformSpecificException();
		};
	}
}

#endif
