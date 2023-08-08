#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

namespace Camel
{
	Mesh Mesh::Load(const std::string& filePath)
	{
		std::string fileExtension = std::filesystem::path(filePath).extension().string();

		if (fileExtension != ".obj")
		{
			CAMEL_LOG_ERROR("Failed to load mesh. Extension {} not supported", fileExtension);
			throw std::runtime_error("Failed to load mesh. Extension " + fileExtension + " not supported");
		}

		std::ifstream file(filePath);
		if (!file.is_open())
		{
			CAMEL_LOG_ERROR("Failed to load obj mesh at path: {}", filePath);
			throw std::runtime_error("Failed to load obj mesh at path: " + filePath);
		}

		std::vector<glm::vec3> temp_positions, temp_normals;
		std::vector<glm::vec2> temp_texCoords;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::string prefix;
			iss >> prefix;

			if (prefix == "v")
			{
				glm::vec3 position{};
				iss >> position.x >> position.y >> position.z;
				temp_positions.push_back(position);
			}
			else if (prefix == "vn")
			{
				glm::vec3 normal{};
				iss >> normal.x >> normal.y >> normal.z;
				temp_normals.push_back(normal);
			}
			else if (prefix == "vt")
			{
				glm::vec2 texCoord{};
				iss >> texCoord.x >> texCoord.y;
				temp_texCoords.push_back(texCoord);
			}
			else if (prefix == "f")
			{
				std::string vertexInfo;
				while (iss >> vertexInfo)
				{
					std::replace(vertexInfo.begin(), vertexInfo.end(), '/', ' ');
					std::istringstream vertexStream(vertexInfo);

					unsigned int posIndex, texIndex, normIndex;
					vertexStream >> posIndex >> texIndex >> normIndex;

					// obj indices are 1-based, so subtract 1 to make them 0-based
					// TODO: You can optimize this by not creating new vertex for each entry as there will be duplicates.
					Vertex vertex = { temp_positions[posIndex - 1], temp_normals[normIndex - 1], temp_texCoords[texIndex - 1] };
					vertices.push_back(vertex);

					// Assuming a triangulated mesh, add indices in order
					indices.push_back((GLuint)indices.size());
				}
			}
		}

		return Mesh(vertices, indices);
	}

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
