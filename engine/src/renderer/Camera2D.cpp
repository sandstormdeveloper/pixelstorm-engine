#include "pixelstorm/renderer/Camera2D.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

Camera2D::Camera2D(float left, float right, float bottom, float top)
    : m_Position(0.0f, 0.0f),
      m_RotationDegrees(0.0f),
      m_ProjectionMatrix(1.0f),
      m_ViewMatrix(1.0f),
      m_ViewProjectionMatrix(1.0f),
      m_ViewDirty(true)
{
    // Updates bounds
    SetProjection(left, right, bottom, top);
}

void Camera2D::SetProjection(float left, float right, float bottom, float top)
{
    // Builds projection matrix
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewDirty = true;
}

void Camera2D::SetPosition(const glm::vec2 &position)
{
    // Updates position
    m_Position = position;
    m_ViewDirty = true;
}

void Camera2D::SetRotation(float rotationDegrees)
{
    // Updates rotation
    m_RotationDegrees = rotationDegrees;
    m_ViewDirty = true;
}

const glm::vec2 &Camera2D::GetPosition() const
{
    // Returns position
    return m_Position;
}

float Camera2D::GetRotation() const
{
    // Returns rotation (degrees)
    return m_RotationDegrees;
}

const glm::mat4 &Camera2D::GetProjectionMatrix() const
{
    // Returns projection matrix
    return m_ProjectionMatrix;
}

const glm::mat4 &Camera2D::GetViewMatrix() const
{
    // Returns view matrix
    RecalculateViewMatrix();
    return m_ViewMatrix;
}

const glm::mat4 &Camera2D::GetViewProjectionMatrix() const
{
    // Returns combined matrix
    RecalculateViewMatrix();
    return m_ViewProjectionMatrix;
}

void Camera2D::RecalculateViewMatrix() const
{
    // Checks if recalculations are needed
    if (!m_ViewDirty)
    {
        return;
    }

    // Builds camera transform
    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));

    // Rebuils matrices
    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    m_ViewDirty = false;
}
