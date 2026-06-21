#include "pch.h"
#include "Frustum.h"

namespace QuadArch {

    Frustum Frustum::FromViewProjection(const glm::mat4& m)
    {
        Frustum frustum;

        frustum.m_Planes[0] = glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]);
        frustum.m_Planes[1] = glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]);
        frustum.m_Planes[2] = glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]);
        frustum.m_Planes[3] = glm::vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]);
        frustum.m_Planes[4] = glm::vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]);
        frustum.m_Planes[5] = glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]);

        for (int i = 0; i < 6; i++)
        {
            float length = glm::length(glm::vec3(frustum.m_Planes[i]));
            if (length > 0.0001f)
                frustum.m_Planes[i] /= length;
        }

        return frustum;
    }

    bool Frustum::IntersectsSphere(const glm::vec3& center, float radius) const
    {
        for (int i = 0; i < 6; i++)
        {
            float distance = glm::dot(glm::vec3(m_Planes[i]), center) + m_Planes[i].w;
            if (distance < -radius)
                return false;
        }
        return true;
    }

    bool Frustum::IntersectsAABB(const glm::vec3& min, const glm::vec3& max) const
    {
        for (int i = 0; i < 6; i++)
        {
            const glm::vec4& plane = m_Planes[i];
            glm::vec3 positiveVertex(
                plane.x >= 0.0f ? max.x : min.x,
                plane.y >= 0.0f ? max.y : min.y,
                plane.z >= 0.0f ? max.z : min.z
            );

            if (glm::dot(glm::vec3(plane), positiveVertex) + plane.w < 0.0f)
                return false;
        }
        return true;
    }
}
