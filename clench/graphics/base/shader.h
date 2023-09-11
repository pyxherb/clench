#ifndef _CLENCH_GRAPHICS_SHADER_H_
#define _CLENCH_GRAPHICS_SHADER_H_

#include <map>

namespace clench {
	namespace graphics {
		enum class ShaderType : std::uint8_t {
			VERTEX = 0,	 // Vertex
			PIXEL,		 // Pixel/Fragment
			GEOMETRY,	 // Geometry
		};

		class RawShader {
		public:
			virtual inline ~RawShader() {}
			virtual ShaderType getType() = 0;
			virtual const char* getSource() = 0;
			virtual std::size_t getSourceSize() = 0;
			virtual std::string getSourceName() = 0;
		};

		class BasicRawShader : public RawShader {
		protected:
			ShaderType _type;

		public:
			inline BasicRawShader(ShaderType type) { _type = type; }
			virtual inline ~BasicRawShader() {}
			virtual ShaderType getType() override { return _type; }
		};

		class MemoryShader : public BasicRawShader {
		protected:
			std::string _src;

		public:
			inline MemoryShader(ShaderType type, std::string src) : BasicRawShader(type) { _src = src; }
			virtual inline ~MemoryShader() {}
			virtual const char* getSource() override { return _src.c_str(); }
			virtual std::size_t getSourceSize() override { return _src.size(); }
			virtual std::string getSourceName() override {
				return "<Memory Shader>: " + std::to_string((std::size_t)_src.c_str());
			}
		};

		class FileShader : public BasicRawShader {
		protected:
			std::string _path;
			std::string _src;

			bool _loaded = false;

			void inline _load() {
				if (_loaded)
					return;

				FILE* fp;
				std::size_t fileSize;

				// Try to open the file
				if (!(fp = fopen(_path.c_str(), "rb")))
					throw std::runtime_error("Error opening file: " + _path);

				// Get file size
				{
					fseek(fp, 0, SEEK_END);
					if ((fileSize = ftell(fp)) < 0) {
						fclose(fp);
						throw std::runtime_error("Error getting file size: " + _path);
					}
					fseek(fp, 0, SEEK_SET);
				}

				_src.resize(fileSize);	// Resize buffer
				// Read the whole file
				if (!fread(&_src[0], fileSize, 1, fp)) {
					fclose(fp);
					throw std::runtime_error("Error reading file: " + _path);
				}
				fclose(fp);

				_loaded = true;
			}

		public:
			inline FileShader(ShaderType type, std::string path) : BasicRawShader(type) { _path = path; }
			virtual inline ~FileShader() {}
			virtual const char* getSource() override {
				_load();
				return _src.c_str();
			}
			virtual std::size_t getSourceSize() override {
				_load();
				return _src.size();
			}
			virtual std::string getSourceName() override {
				return _path;
			}
		};

		class ShaderSet {
		protected:
			std::map<std::string, RawShader> entries;

		public:
		};

		class Shader {
		public:
			virtual ~Shader() = default;
		};

		class ShaderProgram {
		public:
			virtual ~ShaderProgram() = default;
		};

		class ShaderCompilationError : public std::runtime_error {
		protected:
			std::string _details;

		public:
			explicit inline ShaderCompilationError(std::string s, std::string details) : runtime_error(s) {
				_details = details;
			}
			virtual ~ShaderCompilationError() = default;

			virtual std::string details() { return _details; }
		};

		class ShaderLinkingError : public std::runtime_error {
		protected:
			std::string _details;

		public:
			explicit inline ShaderLinkingError(std::string s, std::string details) : runtime_error(s) {
				_details = details;
			}
			virtual ~ShaderLinkingError() = default;

			virtual std::string details() { return _details; }
		};
	}
}

#endif
