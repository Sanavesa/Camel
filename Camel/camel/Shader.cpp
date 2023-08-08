#include "Shader.h"

#include <fstream>
#include <sstream>

namespace Camel
{
	Shader Shader::Load(const std::string& vertexFilePath, const std::string& fragmentFilePath)
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

		return Shader(vertexStream.str(), fragmentStream.str());
	}

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_ShaderID = glCreateProgram();
		if (!m_ShaderID)
		{
			CAMEL_LOG_ERROR("Failed to create shader program");
			throw std::runtime_error("Failed to create shader program");
		}

		GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
		GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		glAttachShader(m_ShaderID, vs);
		glAttachShader(m_ShaderID, fs);
		glLinkProgram(m_ShaderID);

		GLint success;
		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLint length = 0;
			glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)_malloca(length * sizeof(char));
			if (!message)
			{
				CAMEL_LOG_ERROR("Failed to allocate memory for shader link error message");
				throw std::runtime_error("Failed to allocate memory for link error message");
			}

			glGetProgramInfoLog(m_ShaderID, length, &length, message);
			CAMEL_LOG_ERROR("Shader program link error: {}", message);
			throw std::runtime_error("Shader program linking failed");
		}

		glValidateProgram(m_ShaderID);
		glGetProgramiv(m_ShaderID, GL_VALIDATE_STATUS, &success);
		if (!success)
		{
			GLint length = 0;
			glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)_malloca(length * sizeof(char));
			if (!message)
			{
				CAMEL_LOG_ERROR("Failed to allocate memory for shader validation error message");
				throw std::runtime_error("Failed to allocate memory for shader validation error message");
			}

			glGetProgramInfoLog(m_ShaderID, length, &length, message);
			CAMEL_LOG_ERROR("Shader program validation error: {}", message);
			throw std::runtime_error("Shader program validation failed");
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	Shader::Shader(Shader&& other) noexcept
		: m_ShaderID(other.m_ShaderID), m_UniformLocationCache(std::move(other.m_UniformLocationCache))
	{
		other.m_ShaderID = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteProgram(m_ShaderID);

			m_ShaderID = other.m_ShaderID;
			m_UniformLocationCache = std::move(other.m_UniformLocationCache);

			other.m_ShaderID = 0;
		}
		return *this;
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
			CAMEL_LOG_WARN("Uniform {} does not exist or is not being used by the shader", name);

		m_UniformLocationCache.emplace(name, location);

		return location;
	}

	GLuint Shader::CompileShader(const GLenum shaderType, const std::string& source) const
	{
		GLuint id = glCreateShader(shaderType);
		if (!id)
		{
			CAMEL_LOG_ERROR("Failed to create {} shader", (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment"));
			throw std::runtime_error("Failed to create shader");
		}

		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLint length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)_malloca(length * sizeof(char));
			if (!message)
			{
				glDeleteShader(id);

				CAMEL_LOG_ERROR("Failed to allocate memory for shader compile error message");
				throw std::runtime_error("Failed to allocate memory for shader compile error message");
			}

			glGetShaderInfoLog(id, length, &length, message);
			glDeleteShader(id);

			CAMEL_LOG_ERROR("Failed to compile {} shader! Message: {}", (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
			throw std::runtime_error("Failed to compile shader");
		}

		return id;
	}
}