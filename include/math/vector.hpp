#pragma once

#include <glm/vec4.hpp>
#include <iostream>

namespace math
{
inline float norm(glm::vec4 v)
{
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return sqrt(vx * vx + vy * vy + vz * vz);
}

inline glm::vec4 normalize(glm::vec4 v)
{

    return v / norm(v);
}

inline glm::vec4 crossProduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;

    return glm::vec4(u2 * v3 - u3 * v2, u3 * v1 - u1 * v3, u1 * v2 - u2 * v1, 0.0f);
}

inline float dotProduct(glm::vec4 u, glm::vec4 v)
{
    float u1 = u.x;
    float u2 = u.y;
    float u3 = u.z;
    float u4 = u.w;
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;
    float v4 = v.w;

    if (u4 != 0.0f || v4 != 0.0f)
    {
        fprintf(stderr, "ERROR: Scalar product is not defined for points.\n");

        // TODO: create a multi platform stack trace function

        // void *array[10];
        // size_t size;

        // size = backtrace(array, 10);

        // fprintf(stderr, "Error occurred, printing stack:\n");
        // backtrace_symbols_fd(array, size, STDERR_FILENO);

        std::exit(EXIT_FAILURE);
    }

    return u1 * v1 + u2 * v2 + u3 * v3;
}
} // namespace math