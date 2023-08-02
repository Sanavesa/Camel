#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane);

    void SetPosition(const glm::vec3& m_Position);
    void LookAt(const glm::vec3& m_LookAtPoint);

    inline const glm::vec3& GetPosition() const noexcept
    {
        return m_Position;
    }

    inline const glm::mat4& GetViewMatrix() const noexcept
    {
        return m_ViewMatrix;
    }

    inline const glm::mat4& GetProjectionMatrix() const noexcept
    {
        return m_ProjectionMatrix;
    }

private:
    glm::vec3 m_Position;
    glm::vec3 m_LookAtPoint;
    glm::vec3 m_UpVector;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;

    void UpdateViewMatrix();
    void UpdateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
};