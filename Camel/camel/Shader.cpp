#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
{
	CreateShader(vertexSource, fragmentSource);
}

Shader::Shader(const std::string& filePath)
{
	std::ifstream stream(filePath);
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	std::string vertexSource = ss[0].str();
	std::string fragmentSource = ss[1].str();
	CreateShader(vertexSource, fragmentSource);
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
		std::cout << "Warning: uniform " << name << " does not exist." << std::endl;
	
	m_UniformLocationCache.emplace(name, location);

	return location;
}

GLuint Shader::CompileShader(const unsigned int shaderType, const std::string& source) const
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
		std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	m_ShaderID = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	glAttachShader(m_ShaderID, vs);
	glAttachShader(m_ShaderID, fs);
	glLinkProgram(m_ShaderID);
	glValidateProgram(m_ShaderID);
	glDeleteShader(vs);
	glDeleteShader(fs);
}
