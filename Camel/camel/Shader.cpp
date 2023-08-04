#include "Shader.h"

#include <fstream>
#include <sstream>

namespace Camel
{
	Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
		: m_ShaderID(0)
	{
		std::ifstream vertexFile(vertexFilePath);
		if (!vertexFile)
		{
			CAMEL_LOG_ERROR("Failed to load vertex shader at path: {}", vertexFilePath);
			throw std::runtime_error("Failed to load vertex shader at path: " + vertexFilePath);
		}

		std::ifstream fragmentFile(fragmentFilePath);
		if (!fragmentFile)
		{
			CAMEL_LOG_ERROR("Failed to load fragment shader at path: {}", fragmentFilePath);
			throw std::runtime_error("Failed to load fragment shader at path: " + fragmentFilePath);
		}


		std::stringstream vertexStream, fragmentStream;
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		CreateShader(vertexStream.str(), fragmentStream.str());
	}

	Shader::~Shader() noexcept
	{
		glDeleteProgram(m_ShaderID);
	}

	GLint Shader::GetUniformLocation(const std::string& name)
	{
		auto found = m_UniformLocationCache.find(name);
		if (found != m_UniformLocationCache.end())
			return found->second;

		GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
		if (location == -1)
			CAMEL_LOG_WARN("Uniform {} does not exist or is not being used by the shader.", name);

		m_UniformLocationCache.emplace(name, location);

		return location;
	}

	GLuint Shader::CompileShader(const GLenum shaderType, const std::string& source) const
	{
		GLuint id = glCreateShader(shaderType);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			glDeleteShader(id);

			CAMEL_LOG_ERROR("Failed to compile {} shader! Message: {}", (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
			throw std::runtime_error("Failed to compile shader");
		}

		return id;
	}

	void Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_ShaderID = glCreateProgram();
		GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
		GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		glAttachShader(m_ShaderID, vs);
		glAttachShader(m_ShaderID, fs);
		glLinkProgram(m_ShaderID);
		glValidateProgram(m_ShaderID);
		glDeleteShader(vs);
		glDeleteShader(fs);
	}
}