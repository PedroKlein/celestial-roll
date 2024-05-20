#pragma once

#include "matrixUtils.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>

struct CollisionResult
{
    bool collided;
    glm::vec4 normal;
    float penetrationDepth;
};

class CollisionDetector
{
  public:
    static CollisionResult pointSphere(const glm::vec4 &point, const glm::vec4 &sphereCenter, float sphereRadius)
    {
        CollisionResult result{false, glm::vec4(0.0f), 0};

        float distance = sqrt(pow(point.x - sphereCenter.x, 2) + pow(point.y - sphereCenter.y, 2) +
                              pow(point.z - sphereCenter.z, 2));

        if (distance <= sphereRadius)
        {
            result.collided = true;
            result.normal = MatrixUtils::normalize(point - sphereCenter);
            result.penetrationDepth = sphereRadius - distance;
        }

        return result;
    }

    // static bool pointCube(const glm::vec4 &point, const glm::vec4 &cubeMinBounds, const glm::vec4 &cubeMaxBounds)
    // {
    //     bool withinX = point.x >= cubeMinBounds.x && point.x <= cubeMaxBounds.x;
    //     bool withinY = point.y >= cubeMinBounds.y && point.y <= cubeMaxBounds.y;
    //     bool withinZ = point.z >= cubeMinBounds.z && point.z <= cubeMaxBounds.z;

    //     return withinX && withinY && withinZ;
    // }

    static CollisionResult cubeCube(const glm::vec4 &cube1MinBounds, const glm::vec4 &cube1MaxBounds,
                                    const glm::vec4 &cube2MinBounds, const glm::vec4 &cube2MaxBounds)
    {
        CollisionResult result{false, glm::vec4(0.0f), 0};

        float overlapX = std::min(cube1MaxBounds.x, cube2MaxBounds.x) - std::max(cube1MinBounds.x, cube2MinBounds.x);
        float overlapY = std::min(cube1MaxBounds.y, cube2MaxBounds.y) - std::max(cube1MinBounds.y, cube2MinBounds.y);
        float overlapZ = std::min(cube1MaxBounds.z, cube2MaxBounds.z) - std::max(cube1MinBounds.z, cube2MinBounds.z);

        if (overlapX <= 0 || overlapY < 0 || overlapZ < 0)
        {
            return result;
        }

        result.collided = true;

        float minOverlap = std::min({overlapX, overlapY, overlapZ});
        result.penetrationDepth = minOverlap;

        if (minOverlap == overlapX)
        {
            result.normal = (cube1MinBounds.x < cube2MinBounds.x) ? glm::vec4(-1, 0, 0, 0) : glm::vec4(1, 0, 0, 0);
        }
        else if (minOverlap == overlapY)
        {
            result.normal = (cube1MinBounds.y < cube2MinBounds.y) ? glm::vec4(0, -1, 0, 0) : glm::vec4(0, 1, 0, 0);
        }
        else if (minOverlap == overlapZ)
        {
            result.normal = (cube1MinBounds.z < cube2MinBounds.z) ? glm::vec4(0, 0, -1, 0) : glm::vec4(0, 0, 1, 0);
        }

        return result;
    }
};