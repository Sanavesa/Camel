#pragma once

#include "Core.h"

#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

namespace Camel
{
	class Shader
	{
	public:
		Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
		~Shader() noexcept;

		inline void Bind() const noexcept
		{
			glUseProgram(m_ShaderID);
		}

		inline void Unbind() const noexcept
		{
			glUseProgram(0);
		}

		inline void SetUniform1i(const std::string& name, const GLint value) noexcept
		{
			glUniform1i(GetUniformLocation(name), value);
		}

		inline void SetUniform2i(const std::string& name, const GLint v0, const GLint v1) noexcept
		{
			glUniform2i(GetUniformLocation(name), v0, v1);
		}

		inline void SetUniform2i(const std::string& name, const glm::ivec2& value) noexcept
		{
			glUniform2iv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform3i(const std::string& name, const GLint v0, const GLint v1, const GLint v2) noexcept
		{
			glUniform3i(GetUniformLocation(name), v0, v1, v2);
		}

		inline void SetUniform3i(const std::string& name, const glm::ivec3& value) noexcept
		{
			glUniform3iv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform4i(const std::string& name, const GLint v0, const GLint v1, const GLint v2, const GLint v3) noexcept
		{
			glUniform4i(GetUniformLocation(name), v0, v1, v2, v3);
		}

		inline void SetUniform4i(const std::string& name, const glm::ivec4& value) noexcept
		{
			glUniform4iv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform1u(const std::string& name, const GLuint value) noexcept
		{
			glUniform1ui(GetUniformLocation(name), value);
		}

		inline void SetUniform2u(const std::string& name, const GLuint v0, const GLuint v1) noexcept
		{
			glUniform2ui(GetUniformLocation(name), v0, v1);
		}

		inline void SetUniform2u(const std::string& name, const glm::uvec2& value) noexcept
		{
			glUniform2uiv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform3u(const std::string& name, const GLuint v0, const GLuint v1, const GLuint v2) noexcept
		{
			glUniform3ui(GetUniformLocation(name), v0, v1, v2);
		}

		inline void SetUniform3u(const std::string& name, const glm::uvec3& value) noexcept
		{
			glUniform3uiv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform4u(const std::string& name, const GLuint v0, const GLuint v1, const GLuint v2, const GLuint v3) noexcept
		{
			glUniform4ui(GetUniformLocation(name), v0, v1, v2, v3);
		}

		inline void SetUniform4u(const std::string& name, const glm::uvec4& value) noexcept
		{
			glUniform4uiv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform1f(const std::string& name, const GLfloat value) noexcept
		{
			glUniform1f(GetUniformLocation(name), value);
		}

		inline void SetUniform2f(const std::string& name, const GLfloat v0, const GLfloat v1) noexcept
		{
			glUniform2f(GetUniformLocation(name), v0, v1);
		}

		inline void SetUniform2f(const std::string& name, const glm::vec2& value) noexcept
		{
			glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform3f(const std::string& name, const GLfloat v0, const GLfloat v1, const GLfloat v2) noexcept
		{
			glUniform3f(GetUniformLocation(name), v0, v1, v2);
		}

		inline void SetUniform3f(const std::string& name, const glm::vec3& value) noexcept
		{
			glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform4f(const std::string& name, const GLfloat v0, const GLfloat v1, const GLfloat v2, const GLfloat v3) noexcept
		{
			glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
		}

		inline void SetUniform4f(const std::string& name, const glm::vec4& value) noexcept
		{
			glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniformMatrix2f(const std::string& name, const glm::mat2& value) noexcept
		{
			glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void SetUniformMatrix3f(const std::string& name, const glm::mat3& value) noexcept
		{
			glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void SetUniformMatrix4x3f(const std::string& name, const glm::mat4x3& value) noexcept
		{
			glUniformMatrix4x3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void SetUniformMatrix4f(const std::string& name, const glm::mat4& value) noexcept
		{
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void SetUniform1d(const std::string& name, const GLdouble value) noexcept
		{
			glUniform1d(GetUniformLocation(name), value);
		}

		inline void SetUniform2d(const std::string& name, const GLdouble v0, const GLdouble v1) noexcept
		{
			glUniform2d(GetUniformLocation(name), v0, v1);
		}

		inline void SetUniform2d(const std::string& name, const glm::dvec2& value) noexcept
		{
			glUniform2dv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform3d(const std::string& name, const GLdouble v0, const GLdouble v1, const GLdouble v2) noexcept
		{
			glUniform3d(GetUniformLocation(name), v0, v1, v2);
		}

		inline void SetUniform3d(const std::string& name, const glm::dvec3& value) noexcept
		{
			glUniform3dv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniform4d(const std::string& name, const GLdouble v0, const GLdouble v1, const GLdouble v2, const GLdouble v3) noexcept
		{
			glUniform4d(GetUniformLocation(name), v0, v1, v2, v3);
		}

		inline void SetUniform4d(const std::string& name, const glm::dvec4& value) noexcept
		{
			glUniform4dv(GetUniformLocation(name), 1, glm::value_ptr(value));
		}

		inline void SetUniformMatrix2d(const std::string& name, const glm::dmat2& value) noexcept
		{
			glUniformMatrix2dv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void SetUniformMatrix3d(const std::string& name, const glm::dmat3& value) noexcept
		{
			glUniformMatrix3dv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void SetUniformMatrix4x3d(const std::string& name, const glm::dmat4x3& value) noexcept
		{
			glUniformMatrix4x3dv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void SetUniformMatrix4d(const std::string& name, const glm::dmat4& value) noexcept
		{
			glUniformMatrix4dv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
		}

	private:
		GLuint CompileShader(const GLenum shaderType, const std::string& source) const;
		void CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
		GLint GetUniformLocation(const std::string& name);

	private:
		GLuint m_ShaderID;
		std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};
}