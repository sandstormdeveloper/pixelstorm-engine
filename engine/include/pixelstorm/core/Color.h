#pragma once

#include <glm/vec4.hpp>

namespace Colors
{
    inline glm::vec4 White()
    {
        // Returns white color
        return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    inline glm::vec4 Red()
    {
        // Returns demo red color
        return glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);
    }

    inline glm::vec4 Blue()
    {
        // Returns demo blue color
        return glm::vec4(0.3f, 0.6f, 1.0f, 1.0f);
    }

    inline glm::vec4 Green()
    {
        // Returns green color
        return glm::vec4(0.3f, 1.0f, 0.4f, 1.0f);
    }
}
