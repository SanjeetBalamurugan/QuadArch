#pragma once

#include "QuadArch/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace QuadArch {

    class QuadAPI Camera3D
    {
    public:
        Camera3D(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
            float fov = 45.0f, float aspectRatio = 1.777f,
            float nearPlane = 0.1f, float farPlane = 1000.0f);

        ~Camera3D() = default;

        void OnUpdate(float deltaTime, float mouseX, float mouseY);

        void SetAspectRatio(float ratio);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        glm::mat4 GetViewProjectionMatrix() const;

        glm::vec3 GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; }
        float GetDistanceFromCamera(const glm::vec3& position) const { return glm::distance(m_Position, position); }

    private:
        void UpdateCameraVectors();

    private:
        // Transform attributes
        glm::vec3 m_Position;
        glm::vec3 m_Forward;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp;

        float m_Yaw;
        float m_Pitch;

        float m_FOV;
        float m_AspectRatio;
        float m_NearPlane;
        float m_FarPlane;

        float m_MovementSpeed = 5.0f;
        float m_MouseSensitivity = 0.1f;

        float m_LastMouseX = 0.0f;
        float m_LastMouseY = 0.0f;
        bool m_FirstMouseMovement = true;
    };
}
