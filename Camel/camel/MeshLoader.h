#pragma once

#include "Core.h"
#include "Mesh.h"

namespace Camel
{
	class MeshLoader
	{
	public:
		static Mesh Load(const std::string& filePath);
	private:
		static Mesh LoadFromObj(const std::string& filePath);
	};
}