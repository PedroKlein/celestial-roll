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
    static CollisionResult pointToSphere(const glm::vec4 &point, const glm::vec4 &sphereCenter, float sphereRadius)
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

    static CollisionResult sphereToOBB(const glm::vec4 &sphereCenter, float sphereRadius, const glm::vec4 &obbCenter,
                                       const glm::vec3 &obbHalfWidths, const glm::mat3 &obbOrientation)
    {
        // Transform sphere center to OBB's local coordinate system
        glm::vec3 relCenter = glm::vec3(sphereCenter) - glm::vec3(obbCenter);
        glm::vec3 localSphereCenter = obbOrientation * relCenter;

        // Create local AABB bounds for OBB
        glm::vec4 localAABBMin = glm::vec4(-obbHalfWidths, 0.0f);
        glm::vec4 localAABBMax = glm::vec4(obbHalfWidths, 0.0f);

        // Use sphereToAABB method to test collision in local space
        CollisionResult localResult =
            sphereToAABB(glm::vec4(localSphereCenter, 1.0f), sphereRadius, localAABBMin, localAABBMax);

        if (localResult.collided)
        {
            // Transform the collision normal back to world space
            glm::vec3 worldNormal = glm::transpose(obbOrientation) * glm::vec3(localResult.normal);
            localResult.normal = glm::vec4(worldNormal, 0.0f);
        }

        return localResult;
    }

    static CollisionResult sphereToAABB(const glm::vec4 &sphereCenter, float sphereRadius,
                                        const glm::vec4 &aabbMinBounds, const glm::vec4 &aabbMaxBounds)
    {
        CollisionResult result;
        result.collided = false;
        result.normal = glm::vec4(0.0f);
        result.penetrationDepth = 0.0f;

        glm::vec3 closestPoint =
            glm::clamp(glm::vec3(sphereCenter), glm::vec3(aabbMinBounds), glm::vec3(aabbMaxBounds));

        glm::vec3 distance = glm::vec3(sphereCenter) - closestPoint;

        float distSquared = glm::dot(distance, distance);

        if (distSquared < sphereRadius * sphereRadius)
        {
            result.collided = true;
            float dist = sqrt(distSquared);

            result.penetrationDepth = sphereRadius - dist;
            result.normal = glm::vec4(glm::normalize(distance), 0.0f);
        }

        return result;
    }

    static CollisionResult aabbToAABB(const glm::vec4 &aabb1MinBounds, const glm::vec4 &aabb1MaxBounds,
                                      const glm::vec4 &aabb2MinBounds, const glm::vec4 &aabb2MaxBounds)
    {
        CollisionResult result{false, glm::vec4(0.0f), 0};

        float overlapX = std::min(aabb1MaxBounds.x, aabb2MaxBounds.x) - std::max(aabb1MinBounds.x, aabb2MinBounds.x);
        float overlapY = std::min(aabb1MaxBounds.y, aabb2MaxBounds.y) - std::max(aabb1MinBounds.y, aabb2MinBounds.y);
        float overlapZ = std::min(aabb1MaxBounds.z, aabb2MaxBounds.z) - std::max(aabb1MinBounds.z, aabb2MinBounds.z);

        if (overlapX <= 0 || overlapY < 0 || overlapZ < 0)
        {
            return result;
        }

        result.collided = true;

        float minOverlap = std::min({overlapX, overlapY, overlapZ});
        result.penetrationDepth = minOverlap;

        if (minOverlap == overlapX)
        {
            result.normal = (aabb1MinBounds.x < aabb2MinBounds.x) ? glm::vec4(-1, 0, 0, 0) : glm::vec4(1, 0, 0, 0);
        }
        else if (minOverlap == overlapY)
        {
            result.normal = (aabb1MinBounds.y < aabb2MinBounds.y) ? glm::vec4(0, -1, 0, 0) : glm::vec4(0, 1, 0, 0);
        }
        else if (minOverlap == overlapZ)
        {
            result.normal = (aabb1MinBounds.z < aabb2MinBounds.z) ? glm::vec4(0, 0, -1, 0) : glm::vec4(0, 0, 1, 0);
        }

        return result;
    }
};