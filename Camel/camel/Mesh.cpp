#include "Mesh.h"

namespace Camel
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_IBO);

		glBindVertexArray(m_VAO);

		// Copy vertices data
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		// Position (3 floats at index 0, 1, 2)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0);

		// Normal (3 floats at index 3, 4, 5)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);

		// Texcoord (2 floats at index 6, 7)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, texcoord)));
		glEnableVertexAttribArray(2);

		// Copy indices data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_IndexCount = (GLsizei)indices.size();
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: m_VAO(other.m_VAO), m_VBO(other.m_VBO), m_IBO(other.m_IBO), m_IndexCount(other.m_IndexCount)
	{
		other.m_VAO = 0;
		other.m_VBO = 0;
		other.m_IBO = 0;
		other.m_IndexCount = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this != &other)
		{
			// Release any resources we're holding
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
			glDeleteBuffers(1, &m_IBO);

			// Transfer ownership of other's resources to this
			m_VAO = other.m_VAO;
			m_VBO = other.m_VBO;
			m_IBO = other.m_IBO;
			m_IndexCount = other.m_IndexCount;

			// Leave other in a safely destructible state
			other.m_VAO = 0;
			other.m_VBO = 0;
			other.m_IBO = 0;
			other.m_IndexCount = 0;
		}
		return *this;
	}

	Mesh::~Mesh() noexcept
	{
		// Unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);
	}

	void Mesh::Draw() const noexcept
	{
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void Mesh::DrawOutline(GLfloat lineWidth) const noexcept
	{
		CAMEL_ASSERT(lineWidth > 0, "Outline width {} must be positive", lineWidth);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(lineWidth);
		Draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
