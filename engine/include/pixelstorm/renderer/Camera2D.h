#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

class Camera2D
{
public:
    Camera2D(float left = -1.0f, float right = 1.0f, float bottom = -1.0f, float top = 1.0f);

    void SetProjection(float left, float right, float bottom, float top); // Updates orthographic bounds
    void SetPosition(const glm::vec2 &position);                          // Moves the camera in world space
    void SetRotation(float rotationDegrees);                              // Rotates the camera around the Z axis

    const glm::vec2 &GetPosition() const; // Returns camera position
    float GetRotation() const;            // Returns camera rotation in degrees

    const glm::mat4 &GetProjectionMatrix() const;     // Returns orthographic projection matrix
    const glm::mat4 &GetViewMatrix() const;           // Returns view matrix
    const glm::mat4 &GetViewProjectionMatrix() const; // Returns combined view-projection matrix

private:
    void RecalculateViewMatrix() const; // Rebuilds cached view matrix

    glm::vec2 m_Position;    // Camera position
    float m_RotationDegrees; // Camera rotation

    glm::mat4 m_ProjectionMatrix;             // Projection matrix
    mutable glm::mat4 m_ViewMatrix;           // View matrix
    mutable glm::mat4 m_ViewProjectionMatrix; // Combined matrix
    mutable bool m_ViewDirty;                 // Indicates if matrices need to be recalculated
};
