#ifndef _CLENCH_GHAL_SHADER_H_
#define _CLENCH_GHAL_SHADER_H_

#include "resource.h"
#include <string>

namespace clench {
	namespace ghal {
		enum class InputVertexShaderSemanticType : uint8_t {
			None = 0,
			Binormal,
			BlendIndices,
			BlendWeight,
			Color,
			Normal,
			Position,
			TransformPosition,
			PointSize,
			Tangent,
			TextureCoord
		};

		enum class OutputVertexShaderSemanticType : uint8_t {
			None = 0,
			Color,
			Fog,
			Position,
			PointSize,
			TessFactor
		};

		enum class InputFragmentShaderSemanticType : uint8_t {
			None = 0,
			Color,
			TextureCoord,
			VFace,
			VPos
		};

		enum class OutputFragmentShaderSemanticType : uint8_t {
			None = 0,
			Color,
			Depth
		};

		enum class ShaderType {
			Vertex = 0,
			Fragment,
			Geometry
		};

		struct ShaderSourceInfo {
			std::string sourcePath;
		};

		class Shader : public DeviceResource {
		public:
			const ShaderType shaderType;

			CLCGHAL_API Shader(
				Device *ownedDevice,
				ShaderType shaderType);
			CLCGHAL_API virtual ~Shader();
		};

		class VertexShader : public Shader {
		public:
			CLCGHAL_API VertexShader(Device *ownedDevice);
			CLCGHAL_API virtual ~VertexShader();
		};

		class FragmentShader : public Shader {
		public:
			CLCGHAL_API FragmentShader(Device *ownedDevice);
			CLCGHAL_API virtual ~FragmentShader();
		};

		class GeometryShader : public Shader {
		public:
			CLCGHAL_API GeometryShader(Device *ownedDevice);
			CLCGHAL_API virtual ~GeometryShader();
		};

		enum class ShaderElementType : uint8_t {
			Int = 0,
			UInt,
			Short,
			UShort,
			Long,
			ULong,
			Float,
			Double,
			Half,
			Boolean
		};

		struct ShaderDataType {
			ShaderElementType elementType;
			uint8_t columns, rows;
			uint32_t nElements;

			PEFF_FORCEINLINE bool operator<(const ShaderDataType &rhs) {
				if (elementType < rhs.elementType)
					return true;
				if (elementType > rhs.elementType)
					return false;
				if (columns > rhs.columns)
					return false;
				if (columns < rhs.columns)
					return true;
				if (rows > rhs.rows)
					return false;
				if (rows < rhs.rows)
					return true;
				return nElements < rhs.nElements;
			}
		};

		class ShaderProgram : public DeviceResource {
		public:
			CLCGHAL_API ShaderProgram(
				Device *ownerDevice);
			CLCGHAL_API virtual ~ShaderProgram();
		};
	}
}

#endif
