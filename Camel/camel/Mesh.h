#pragma once

#include "Core.h"
#include <vector>

namespace Camel
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;

		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;

		~Mesh() noexcept;

		void Draw() const noexcept;
		void DrawOutline(GLfloat lineWidth = 3.0f) const noexcept;

	private:
		GLuint m_VAO, m_VBO, m_IBO;
		GLsizei m_IndexCount;
	};
}
