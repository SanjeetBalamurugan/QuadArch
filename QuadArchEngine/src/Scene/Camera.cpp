#include "pch.h"
#include "Camera.h"

#include "Inputs/Input.h"

QuadArch::Camera3D::Camera3D(glm::vec3 position, float fov, float aspectRatio, float nearPlane, float farPlane)
    : m_Position(position), m_FOV(fov), m_AspectRatio(aspectRatio),
    m_NearPlane(nearPlane), m_FarPlane(farPlane),
    m_Forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_Yaw(-90.0f), m_Pitch(0.0f)
{
    UpdateCameraVectors();
}

void QuadArch::Camera3D::OnUpdate(float deltaTime, float mouseX, float mouseY)
{
    if (m_FirstMouseMovement)
    {
        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;
        m_FirstMouseMovement = false;
    }

    float offsetX = mouseX - m_LastMouseX;
    float offsetY = m_LastMouseY - mouseY;

    m_LastMouseX = mouseX;
    m_LastMouseY = mouseY;

    m_Yaw += offsetX * m_MouseSensitivity;
    m_Pitch += offsetY * m_MouseSensitivity;

    if (m_Pitch > 89.0f)  m_Pitch = 89.0f;
    if (m_Pitch < -89.0f) m_Pitch = -89.0f;

    float velocity = m_MovementSpeed * deltaTime;

    if (Input::IsKeyHeld(Key::W))
        m_Position += m_Forward * velocity;
    if (Input::IsKeyHeld(Key::S))
        m_Position -= m_Forward * velocity;
    if (Input::IsKeyHeld(Key::A))
        m_Position -= m_Right * velocity;
    if (Input::IsKeyHeld(Key::D))
        m_Position += m_Right * velocity;
    if (Input::IsKeyHeld(Key::E)) // Fly Up
        m_Position += m_WorldUp * velocity;
    if (Input::IsKeyHeld(Key::Q)) // Fly Down
        m_Position -= m_WorldUp * velocity;

    UpdateCameraVectors();
}

void QuadArch::Camera3D::SetAspectRatio(float ratio)
{
    m_AspectRatio = ratio;
}

glm::mat4 QuadArch::Camera3D::GetViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

glm::mat4 QuadArch::Camera3D::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
}

glm::mat4 QuadArch::Camera3D::GetViewProjectionMatrix() const
{
    return GetProjectionMatrix() * GetViewMatrix();
}

void QuadArch::Camera3D::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Forward = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}
