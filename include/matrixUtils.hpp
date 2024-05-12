#pragma once

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class MatrixUtils
{
  public:
    static glm::mat4 Matrix(float m00, float m01, float m02, float m03, // Line 1
                            float m10, float m11, float m12, float m13, // Line 2
                            float m20, float m21, float m22, float m23, // Line 3
                            float m30, float m31, float m32, float m33  // Line 4
    )
    {
        return glm::mat4(m00, m10, m20, m30, // Column 1
                         m01, m11, m21, m31, // Column 2
                         m02, m12, m22, m32, // Column 3
                         m03, m13, m23, m33  // Column 4
        );
    }

    static glm::mat4 identityMatrix()
    {
        return Matrix(1.0f, 0.0f, 0.0f, 0.0f, // Line 1
                      0.0f, 1.0f, 0.0f, 0.0f, // Line 2
                      0.0f, 0.0f, 1.0f, 0.0f, // Line 3
                      0.0f, 0.0f, 0.0f, 1.0f  // Line 4
        );
    }

    static glm::mat4 translateMatrix(float tx, float ty, float tz)
    {
        return Matrix(1.0f, 0.0f, 0.0f, tx,  // Line 1
                      0.0f, 1.0f, 0.0f, ty,  // Line 2
                      0.0f, 0.0f, 1.0f, tz,  // Line 3
                      0.0f, 0.0f, 0.0f, 1.0f // Line 4
        );
    }

    static glm::mat4 scaleMatrix(float sx, float sy, float sz)
    {
        return Matrix(sx, 0.0f, 0.0f, 0.0f,  // Line 1
                      0.0f, sy, 0.0f, 0.0f,  // Line 2
                      0.0f, 0.0f, sz, 0.0f,  // Line 3
                      0.0f, 0.0f, 0.0f, 1.0f // Line 4
        );
    }

    static glm::mat4 rotateXMatrix(float angle)
    {
        float c = cos(angle);
        float s = sin(angle);
        return Matrix(1, 0.0f, 0.0f, 0.0f,   // Line 1
                      0.0f, c, -s, 0.0f,     // Line 2
                      0.0f, s, c, 0.0f,      // Line 3
                      0.0f, 0.0f, 0.0f, 1.0f // Line 4
        );
    }

    static glm::mat4 rotateYMatrix(float angle)
    {
        float c = cos(angle);
        float s = sin(angle);
        return Matrix(c, 0.0f, s, 0.0f,       // Line 1
                      0.0f, 1.0f, 0.0f, 0.0f, // Line 2
                      -s, 0.0f, c, 0.0f,      // Line 3
                      0.0f, 0.0f, 0.0f, 1.0f  // Line 4
        );
    }

    static glm::mat4 rotateZMatrix(float angle)
    {
        float c = cos(angle);
        float s = sin(angle);
        return Matrix(c, -s, 0.0f, 0.0f,      // Line 1
                      s, c, 0.0f, 0.0f,       // Line 2
                      0.0f, 0.0f, 1.0f, 0.0f, // Line 3
                      0.0f, 0.0f, 0.0f, 1.0f  // Line 4
        );
    }

    static float norm(glm::vec4 v)
    {
        float vx = v.x;
        float vy = v.y;
        float vz = v.z;

        return sqrt(vx * vx + vy * vy + vz * vz);
    }

    static glm::vec4 normalize(glm::vec4 v)
    {

        return v / norm(v);
    }

    static glm::vec4 crossProduct(glm::vec4 u, glm::vec4 v)
    {
        float u1 = u.x;
        float u2 = u.y;
        float u3 = u.z;
        float v1 = v.x;
        float v2 = v.y;
        float v3 = v.z;

        return glm::vec4(u2 * v3 - u3 * v2, u3 * v1 - u1 * v3, u1 * v2 - u2 * v1, 0.0f);
    }

    static float dotProduct(glm::vec4 u, glm::vec4 v)
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
            std::exit(EXIT_FAILURE);
        }

        return u1 * v1 + u2 * v2 + u3 * v3;
    }
};