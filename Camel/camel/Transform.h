#pragma once

#include "Core.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Camel
{
	class Transform
	{
	public:
		Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1, 0, 0, 0), const glm::vec3& scale = glm::vec3(1.0f))
			: m_Position(position), m_Rotation(rotation), m_Scale(scale), m_IsDirty(true), m_Matrix(glm::mat4(0.0f))
		{}
		~Transform() = default;

		inline const glm::mat4& GetMatrix() const noexcept
		{
			// Recompute lazily when dirty
			if (m_IsDirty)
			{
				m_Matrix = CalculateMatrix();
				m_IsDirty = false;
			}
			return m_Matrix;
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

		inline void Translate(const glm::vec3& delta) noexcept { SetPosition(m_Position + delta); }
		inline void Translate(const float dx, const float dy, const float dz) noexcept { Translate(glm::vec3(dx, dy, dz)); }

		inline void Rotate(const glm::quat& rotation) noexcept { SetRotation(rotation * m_Rotation); }
		inline void Rotate(const glm::vec3& eulerAngles) noexcept { Rotate(glm::quat(eulerAngles)); }
		inline void Rotate(const float eulerX, const float eulerY, const float eulerZ) noexcept { Rotate(glm::vec3(eulerX, eulerY, eulerZ)); }

		inline void Scale(const glm::vec3& factors) noexcept { SetScale(m_Scale * factors); }
		inline void Scale(const float dx, const float dy, const float dz) noexcept { Scale(glm::vec3(dx, dy, dz)); }

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
		virtual inline glm::mat4 CalculateMatrix() const noexcept
		{
			return glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Rotation) * glm::scale(glm::mat4(1.0f), m_Scale);
		}

	private:
		glm::vec3 m_Position, m_Scale;
		glm::quat m_Rotation;
		mutable glm::mat4x4 m_Matrix; // Lazily computed
		mutable bool m_IsDirty; // Used to cache matrix and only recompute when needed
	};

	class CameraTransform : public Transform
	{
	public:
		CameraTransform(const glm::vec3& position = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1, 0, 0, 0))
			: Transform(position, rotation)
		{}

	protected:
		inline glm::mat4 CalculateMatrix() const noexcept
		{
			return glm::lookAtLH(GetPosition(), GetPosition() + GetForward(), GetUp());
		}
	};
}
