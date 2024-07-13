#pragma once

#include <glm/mat4x4.hpp>

namespace math
{

inline glm::mat4 Matrix(float m00, float m01, float m02, float m03, // Line 1
                        float m10, float m11, float m12, float m13, // Line 2
                        float m20, float m21, float m22, float m23, // Line 3
                        float m30, float m31, float m32, float m33  // Line 4
)
{
    return {m00, m10, m20, m30, // Column 1
                     m01, m11, m21, m31, // Column 2
                     m02, m12, m22, m32, // Column 3
                     m03, m13, m23, m33  // Column 4
    };
}

inline glm::mat4 identityMatrix()
{
    return Matrix(1.0f, 0.0f, 0.0f, 0.0f, // Line 1
                  0.0f, 1.0f, 0.0f, 0.0f, // Line 2
                  0.0f, 0.0f, 1.0f, 0.0f, // Line 3
                  0.0f, 0.0f, 0.0f, 1.0f  // Line 4
    );
}

inline glm::mat4 translateMatrix(const float tx, const float ty, const float tz)
{
    return Matrix(1.0f, 0.0f, 0.0f, tx,  // Line 1
                  0.0f, 1.0f, 0.0f, ty,  // Line 2
                  0.0f, 0.0f, 1.0f, tz,  // Line 3
                  0.0f, 0.0f, 0.0f, 1.0f // Line 4
    );
}

inline glm::mat4 scaleMatrix(const float sx, const float sy, const float sz)
{
    return Matrix(sx, 0.0f, 0.0f, 0.0f,  // Line 1
                  0.0f, sy, 0.0f, 0.0f,  // Line 2
                  0.0f, 0.0f, sz, 0.0f,  // Line 3
                  0.0f, 0.0f, 0.0f, 1.0f // Line 4
    );
}

inline glm::mat4 rotateXMatrix(const float angle)
{
    const float c = cos(angle);
    const float s = sin(angle);
    return Matrix(1, 0.0f, 0.0f, 0.0f,   // Line 1
                  0.0f, c, -s, 0.0f,     // Line 2
                  0.0f, s, c, 0.0f,      // Line 3
                  0.0f, 0.0f, 0.0f, 1.0f // Line 4
    );
}

inline glm::mat4 rotateYMatrix(const float angle)
{
    const float c = cos(angle);
    const float s = sin(angle);
    return Matrix(c, 0.0f, s, 0.0f,       // Line 1
                  0.0f, 1.0f, 0.0f, 0.0f, // Line 2
                  -s, 0.0f, c, 0.0f,      // Line 3
                  0.0f, 0.0f, 0.0f, 1.0f  // Line 4
    );
}

inline glm::mat4 rotateZMatrix(const float angle)
{
    const float c = cos(angle);
    const float s = sin(angle);
    return Matrix(c, -s, 0.0f, 0.0f,      // Line 1
                  s, c, 0.0f, 0.0f,       // Line 2
                  0.0f, 0.0f, 1.0f, 0.0f, // Line 3
                  0.0f, 0.0f, 0.0f, 1.0f  // Line 4
    );
}

inline glm::mat4 transposeHomogeneous(glm::mat4 m)
{
    glm::mat4 result = m;
    // transpose only the top-left 3x3 part of the matrix
    result[0][1] = m[1][0];
    result[0][2] = m[2][0];
    result[1][0] = m[0][1];
    result[1][2] = m[2][1];
    result[2][0] = m[0][2];
    result[2][1] = m[1][2];

    return result;
}

inline glm::mat4 orthographicMatrix(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
    return Matrix(2 / (right - left), 0.0f, 0.0f, -(right + left) / (right - left), // Line 1
                  0.0f, 2 / (top - bottom), 0.0f, -(top + bottom) / (top - bottom), // Line 2
                  0.0f, 0.0f, 2 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear), // Line 3
                  0.0f, 0.0f, 0.0f, 1.0f                                            // Line 4
    );
}

inline glm::mat4 perspectiveMatrix(const float fieldOfView, const float aspect, const float zNear, const float zFar)
{
    const float t = fabs(zNear) * tanf(fieldOfView / 2.0f);
    const float b = -t;
    const float r = t * aspect;
    const float l = -r;

    const glm::mat4 P = Matrix(zNear, 0.0f, 0.0f, 0.0f,                 // Line 1
                               0.0f, zNear, 0.0f, 0.0f,                 // Line 2
                               0.0f, 0.0f, zNear + zFar, -zFar * zNear, // Line 3
                               0.0f, 0.0f, 1, 0.0f                      // Line 4
    );

    const glm::mat4 M = orthographicMatrix(l, r, b, t, zNear, zFar);

    return -M * P;
}

} // namespace math