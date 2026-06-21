#pragma once
#include "QuadArch/Core.h"
#include <glm/glm.hpp>

namespace QuadArch {

    class QuadAPI Frustum
    {
    public:
        Frustum() = default;

        static Frustum FromViewProjection(const glm::mat4& viewProjection);

        bool IntersectsSphere(const glm::vec3& center, float radius) const;
        bool IntersectsAABB(const glm::vec3& min, const glm::vec3& max) const;

    private:
        glm::vec4 m_Planes[6] = {};
    };
}
