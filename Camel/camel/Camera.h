#pragma once

#include "Core.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Camel
{
	class Camera final
	{
	public:
		Camera(const glm::vec3& position, const glm::quat& rotation, const float fov = 90.0f, const float aspectRatio = 16.0f / 9.0f, const float nearPlane = 0.1f, const float farPlane = 1000.0f)
			: m_Transform(position, rotation), m_FOV(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane), m_IsDirty(true), m_ProjectionMatrix(glm::mat4(0.0f))
		{
			CAMEL_ASSERT(fov > 0, "Camera fov {} must be positive.", fov);
			CAMEL_ASSERT(aspectRatio > 0, "Camera aspectRatio {} must be positive.", aspectRatio);
			CAMEL_ASSERT(farPlane > 0, "Camera farPlane {} must be positive.", farPlane);
			CAMEL_ASSERT(nearPlane > 0, "Camera nearPlane {} must be positive.", nearPlane);
			CAMEL_ASSERT(nearPlane < farPlane, "Camera nearPlane {} must be less than farPlane {}.", nearPlane, farPlane);
		}
		~Camera() = default;

		inline Transform& GetTransform() noexcept { return m_Transform; }
		inline const Transform& GetTransform() const noexcept { return m_Transform; }

		inline const glm::mat4& GetViewMatrix() const noexcept { return m_Transform.GetMatrix(); }

		inline const glm::mat4& GetProjectionMatrix() const noexcept
		{
			// Recompute lazily when dirty
			if (m_IsDirty)
			{
				m_ProjectionMatrix = glm::perspectiveLH(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
				m_IsDirty = false;
			}
			return m_ProjectionMatrix;
		}

		inline float GetFOV() const noexcept { return m_FOV; }
		inline void SetFOV(const float fov) noexcept
		{
			m_FOV = fov;
			m_IsDirty = true;
		}

		inline float GetAspectRatio() const noexcept { return m_AspectRatio; }
		inline void SetAspectRatio(const float aspectRatio) noexcept
		{
			m_AspectRatio = aspectRatio;
			m_IsDirty = true;
		}

		inline float GetNearPlane() const noexcept { return m_NearPlane; }
		inline void SetNearPlane(const float nearPlane) noexcept
		{
			m_NearPlane = nearPlane;
			m_IsDirty = true;
		}

		inline float GetFarPlane() const noexcept { return m_FarPlane; }
		inline void SetFarPlane(const float FarPlane) noexcept
		{
			m_FarPlane = FarPlane;
			m_IsDirty = true;
		}

	private:
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;

		CameraTransform m_Transform;

		mutable glm::mat4 m_ProjectionMatrix; // Lazily computed
		mutable bool m_IsDirty; // Used to cache projection matrix and only recompute when needed
	};
}