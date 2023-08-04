#pragma once

#include "Core.h"
#include "Transform.h"

namespace Camel
{
	class Light
	{
	public:
		Light(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& color = glm::vec3(1.0f))
			: m_Transform(position), m_Color(color)
		{}

		inline const Transform& GetTransform() const noexcept { return m_Transform; }
		inline Transform& GetTransform() noexcept { return m_Transform; }

		inline const glm::vec3& GetColor() const noexcept { return m_Color; }
		inline glm::vec3& GetColor() noexcept { return m_Color; }

	private:
		Transform m_Transform;
		glm::vec3 m_Color;
	};
}