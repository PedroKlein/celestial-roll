#pragma once

#include <glm/vec4.hpp>
#include <iostream>

namespace math
{
inline float norm(const glm::vec4 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline glm::vec4 normalize(glm::vec4 v)
{
    return v / norm(v);
}

inline glm::vec4 crossProduct(const glm::vec4 u, const glm::vec4 v)
{
    return {u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x, 0.0f};
}

inline float dotProduct(glm::vec4 u, glm::vec4 v)
{
    if (u.w != 0.0f || v.w != 0.0f)
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

    return u.x * v.x + u.y * v.y + u.z * v.z;
}
} // namespace math