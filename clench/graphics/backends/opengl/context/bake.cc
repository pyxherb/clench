#include <typeinfo>

#include "../context.h"

using namespace clench::graphics;

Texture* GLContext::bakeTexture(RawTexture* rawTexture) {
	GLenum fmt = GL_NONE;

	// Map formats
	switch (rawTexture->getFormat()) {
		case TextureFormat::RGB8:
			fmt = GL_RGB;
			break;
		case TextureFormat::RGBA8:
			fmt = GL_RGBA;
			break;
		case TextureFormat::RGBA32F:
			fmt = GL_RGBA;
			break;
		default:
			throw std::runtime_error("Unsupported texture format");
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	{
		auto size = rawTexture->getTextureSize();
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA8,
			size.x(), size.y(),
			0,
			fmt,
			GL_UNSIGNED_BYTE,
			rawTexture->getData());
	}

	return new GLTexture(texture);
}

Shader* GLContext::compileShader(RawShader* rawShader) {
	GLuint shader;
	switch (rawShader->getType()) {
		case ShaderType::VERTEX:
			shader = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::PIXEL:
			shader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::GEOMETRY:
			shader = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		default:
			throw std::logic_error("Unsupported shader type");
	}

	// Compile the source
	{
		GLint szShaderSrc = (GLint)rawShader->getSourceSize();
		const GLchar* shaderSrc = (const GLchar*)rawShader->getSource();
		glShaderSource(shader, 1, &shaderSrc, &szShaderSrc);
		glCompileShader(shader);
	}

	// Check if the shader was compiled successfully
	{
		GLint succeed;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &succeed);

		if (succeed != GL_TRUE) {
			char log[512];
			glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
			throw ShaderCompilationError("Error compiling shader", log);
		}
	}

	return new GLShader(shader);
}

ShaderProgram* GLContext::linkShader(std::vector<Shader*> shaders) {
	GLuint program = glCreateProgram();

	try {
		for (auto i : shaders) {
			glAttachShader(program, ((GLShader*)i)->handle);
		}

		glLinkProgram(program);

		{
			GLint succeed;
			glGetProgramiv(program, GL_LINK_STATUS, &succeed);

			if (succeed != GL_TRUE) {
				char log[512];
				glGetProgramInfoLog(program, sizeof(log), nullptr, log);
				throw ShaderLinkingError("Error linking shader program", log);
			}
		}
	} catch (...) {
		glDeleteProgram(program);
		std::rethrow_exception(std::current_exception());
	}

	return new GLShaderProgram(program);
}
