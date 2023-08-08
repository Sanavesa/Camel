#pragma once

#include "Core.h"
#include "Transform.h"

namespace Camel
{
	class Light final
	{
	public:
		Light(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& color = glm::vec3(1.0f))
			: m_Transform(position), m_Color(color)
		{}

		Light(const Light&) = delete;
		Light& operator=(const Light&) = delete;

		Light(Light&& other) noexcept
			: m_Transform(std::move(other.m_Transform)), m_Color(std::move(other.m_Color))
		{}

		Light& operator=(Light&& other) noexcept
		{
			if (this != &other)
			{
				m_Transform = std::move(other.m_Transform);
				m_Color = std::move(other.m_Color);
			}
			return *this;
		}

		~Light() = default;

		inline const Transform& GetTransform() const noexcept { return m_Transform; }
		inline Transform& GetTransform() noexcept { return m_Transform; }

		inline const glm::vec3& GetColor() const noexcept { return m_Color; }
		inline glm::vec3& GetColor() noexcept { return m_Color; }

	private:
		Transform m_Transform;
		glm::vec3 m_Color;
	};
}