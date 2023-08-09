#pragma once

#include "Core.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Camel
{
	class Transform final
	{
	public:
		enum class Space
		{
			WORLD,
			LOCAL,
		};

	public:
		Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1, 0, 0, 0), const glm::vec3& scale = glm::vec3(1.0f))
			: m_Position(position), m_Rotation(rotation), m_Scale(scale), m_IsDirty(true), m_LocalToWorldMatrix(glm::mat4(0.0f)), m_WorldToLocalMatrix(glm::mat4(0.0f))
		{}

		~Transform() = default;

		inline const glm::mat4& GetLocalToWorldMatrix() const noexcept
		{
			// Recompute lazily when dirty
			if (m_IsDirty)
			{
				m_LocalToWorldMatrix = CalculateLocalToWorldMatrix();
				m_WorldToLocalMatrix = glm::inverse(m_LocalToWorldMatrix);
				m_IsDirty = false;
			}
			return m_LocalToWorldMatrix;
		}

		inline const glm::mat4& GetWorldToLocalMatrix() const noexcept
		{
			// Recompute lazily when dirty
			if (m_IsDirty)
			{
				m_LocalToWorldMatrix = CalculateLocalToWorldMatrix();
				m_WorldToLocalMatrix = glm::inverse(m_LocalToWorldMatrix);
				m_IsDirty = false;
			}
			return m_WorldToLocalMatrix;
		}

		inline const glm::vec3& GetPosition() const noexcept { return m_Position; }
		inline void SetPosition(const glm::vec3& position) noexcept
		{
			m_Position = position;
			m_IsDirty = true;
		}

		inline const glm::quat& GetRotation() const noexcept { return m_Rotation; }
		inline void SetRotation(const glm::quat& rotation) noexcept
		{
			m_Rotation = rotation;
			m_IsDirty = true;
		}

		inline const glm::vec3& GetScale() const noexcept { return m_Scale; }
		inline void SetScale(const glm::vec3& scale) noexcept
		{
			m_Scale = scale;
			m_IsDirty = true;
		}

		inline glm::vec3 TransformPoint(const glm::vec3& point) const noexcept { return glm::vec3(GetLocalToWorldMatrix() * glm::vec4(point, 1.0f)); }
		inline glm::vec3 TransformDirection(const glm::vec3& direction) const noexcept { return glm::vec3(GetLocalToWorldMatrix() * glm::vec4(direction, 0.0f)); }

		inline glm::vec3 InverseTransformPoint(const glm::vec3& point) const noexcept { return glm::vec3(GetWorldToLocalMatrix() * glm::vec4(point, 1.0f)); }
		inline glm::vec3 InverseTransformDirection(const glm::vec3& direction) const noexcept { return glm::vec3(GetWorldToLocalMatrix() * glm::vec4(direction, 0.0f)); }

		inline void Translate(const glm::vec3& delta, const Space space = Space::WORLD) noexcept
		{
			if (space == Space::WORLD)
				SetPosition(m_Position + delta);
			else
				SetPosition(m_Position + TransformDirection(delta));
		}

		inline void Translate(const float dx, const float dy, const float dz, const Space space = Space::WORLD) noexcept { Translate(glm::vec3(dx, dy, dz), space); }

		inline void Rotate(const glm::quat& rotation, const Space space = Space::WORLD) noexcept
		{
			if (space == Space::WORLD)
				SetRotation(rotation * m_Rotation);
			else
				SetRotation(m_Rotation * rotation);
		}
		inline void Rotate(const glm::vec3& eulerAngles, const Space space = Space::WORLD) noexcept { Rotate(glm::quat(eulerAngles)); }
		inline void Rotate(const float eulerX, const float eulerY, const float eulerZ, const Space space = Space::WORLD) noexcept { Rotate(glm::vec3(eulerX, eulerY, eulerZ)); }

		inline void RotateAround(const glm::vec3& pivot, const glm::quat& rotation) noexcept
		{
			const glm::vec3 direction = rotation * (m_Position - pivot);
			SetPosition(pivot + direction);
			LookAt(pivot);
		}

		inline void RotateAround(const glm::vec3& pivot, const glm::vec3& eulerAngles) noexcept
		{
			RotateAround(pivot, glm::quat(eulerAngles));
		}

		inline void RotateAround(const glm::vec3& pivot, const float eulerX, const float eulerY, const float eulerZ) noexcept
		{
			RotateAround(pivot, glm::vec3(eulerX, eulerY, eulerZ));
		}

		inline void Scale(const glm::vec3& factors) noexcept { SetScale(m_Scale * factors); }
		inline void Scale(const float dx, const float dy, const float dz) noexcept { Scale(glm::vec3(dx, dy, dz)); }
		inline void Scale(const float factor) noexcept { Scale(glm::vec3(factor)); }

		inline void LookAt(const glm::vec3& targetPosition, const glm::vec3& upDirection = glm::vec3(0.0f, 1.0f, 0.0f)) noexcept
		{
			glm::mat4 lookAtMatrix = glm::lookAtLH(m_Position, targetPosition, upDirection);
			SetRotation(glm::quat_cast(glm::transpose(lookAtMatrix)));
		}

		inline glm::vec3 GetForward() const noexcept { return m_Rotation * glm::vec3(0.0f, 0.0f, 1.0f); }
		inline glm::vec3 GetBack() const noexcept { return m_Rotation * glm::vec3(0.0f, 0.0f, -1.0f); }
		inline glm::vec3 GetRight() const noexcept { return m_Rotation * glm::vec3(1.0f, 0.0f, 0.0f); }
		inline glm::vec3 GetLeft() const noexcept { return m_Rotation * glm::vec3(-1.0f, 0.0f, 0.0f); }
		inline glm::vec3 GetUp() const noexcept { return m_Rotation * glm::vec3(0.0f, 1.0f, 0.0f); }
		inline glm::vec3 GetDown() const noexcept { return m_Rotation * glm::vec3(0.0f, -1.0f, 0.0f); }

		inline void SetForward(const glm::vec3& forward) noexcept { LookAt(m_Position + glm::normalize(forward)); }

		inline void SetUp(const glm::vec3& up) noexcept
		{
			// Compute the forward direction, which should be orthogonal to the new up direction
			glm::vec3 forward = glm::cross(GetRight(), glm::normalize(up));
			LookAt(m_Position + forward);
		}

		inline void SetRight(const glm::vec3& right) noexcept
		{
			// Compute the forward direction, which should be orthogonal to the new right direction
			glm::vec3 forward = glm::cross(glm::normalize(right), GetUp());
			LookAt(m_Position + forward);
		}

		inline void SetBack(const glm::vec3& back) noexcept { SetForward(-back); }
		inline void SetLeft(const glm::vec3& left) noexcept { SetRight(-left); }
		inline void SetDown(const glm::vec3& down) noexcept { SetUp(-down); }

	protected:
		inline glm::mat4 CalculateLocalToWorldMatrix() const noexcept
		{
			return glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Rotation) * glm::scale(glm::mat4(1.0f), m_Scale);
		}

	private:
		glm::vec3 m_Position, m_Scale;
		glm::quat m_Rotation;
		mutable glm::mat4x4 m_LocalToWorldMatrix; // Lazily computed
		mutable glm::mat4x4 m_WorldToLocalMatrix; // Lazily computed
		mutable bool m_IsDirty; // Used to cache matrix and only recompute when needed
	};
}
