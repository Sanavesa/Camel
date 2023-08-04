#include "MeshLoader.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

namespace Camel
{
	Mesh MeshLoader::Load(const std::string& filePath)
	{
		std::string fileExtension = std::filesystem::path(filePath).extension().string();
		//std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::std::tolower);

		if (fileExtension == ".obj")
			return LoadFromObj(filePath);

		CAMEL_LOG_ERROR("Failed to load mesh. Extension {} not supported", fileExtension);
		throw std::runtime_error("Failed to load mesh. Extension " + fileExtension + " not supported");
	}

	Mesh MeshLoader::LoadFromObj(const std::string& filePath)
	{
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
}
