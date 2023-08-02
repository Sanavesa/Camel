#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_Position = glm::vec3(0.0f, 3.0f, -5.0f);
    m_LookAtPoint = glm::vec3(0.0f);
    m_UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    UpdateProjectionMatrix(fov, aspectRatio, nearPlane, farPlane);
    UpdateViewMatrix();
}

void Camera::SetPosition(const glm::vec3& m_Position)
{
    this->m_Position = m_Position;
    UpdateViewMatrix();
}

void Camera::LookAt(const glm::vec3& m_LookAtPoint)
{
    this->m_LookAtPoint = m_LookAtPoint;
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_LookAtPoint, m_UpVector);
}

void Camera::UpdateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_ProjectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
