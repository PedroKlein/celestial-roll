#pragma once

#include "matrixUtils.hpp"
#include <glm/glm.hpp>
#include <iostream>

class CollisionDetector
{
  public:
    static bool pointSphere(const glm::vec4 &point, const glm::vec4 &sphereCenter, float sphereRadius)
    {
        float distance = sqrt(pow(point.x - sphereCenter.x, 2) + pow(point.y - sphereCenter.y, 2) +
                              pow(point.z - sphereCenter.z, 2));

        return distance <= sphereRadius;
    }

    static bool pointCube(const glm::vec4 &point, const glm::vec4 &cubeMinBounds, const glm::vec4 &cubeMaxBounds)
    {
        bool withinX = point.x >= cubeMinBounds.x && point.x <= cubeMaxBounds.x;
        bool withinY = point.y >= cubeMinBounds.y && point.y <= cubeMaxBounds.y;
        bool withinZ = point.z >= cubeMinBounds.z && point.z <= cubeMaxBounds.z;

        return withinX && withinY && withinZ;
    }

    static bool cubeCube(const glm::vec4 &cube1MinBounds, const glm::vec4 &cube1MaxBounds,
                         const glm::vec4 &cube2MinBounds, const glm::vec4 &cube2MaxBounds)
    {
        bool overlapX = cube1MinBounds.x <= cube2MaxBounds.x && cube1MaxBounds.x >= cube2MinBounds.x;
        bool overlapY = cube1MinBounds.y <= cube2MaxBounds.y && cube1MaxBounds.y >= cube2MinBounds.y;
        bool overlapZ = cube1MinBounds.z <= cube2MaxBounds.z && cube1MaxBounds.z >= cube2MinBounds.z;

        return overlapX && overlapY && overlapZ;
    }
};