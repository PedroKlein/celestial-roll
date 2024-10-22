#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "math/matrix.hpp"
#include "math/vector.hpp"

struct CollisionResult {
    bool collided;
    glm::vec4 normal;
    float penetrationDepth;
};

class CollisionDetector {
public:
    static CollisionResult pointToSphere(const glm::vec4 &point, const glm::vec4 &sphereCenter,
                                         const float sphereRadius) {
        CollisionResult result{false, glm::vec4(0.0f), 0};

        double distance = sqrt(pow(point.x - sphereCenter.x, 2) + pow(point.y - sphereCenter.y, 2) +
                               pow(point.z - sphereCenter.z, 2));

        if (distance <= sphereRadius) {
            result.collided = true;
            result.normal = math::normalize(point - sphereCenter);
            result.penetrationDepth = sphereRadius - distance;
        }

        return result;
    }

    static CollisionResult sphereToOBB(const glm::vec4 &sphereCenter, float sphereRadius, const glm::vec4 &obbCenter,
                                       const glm::vec3 &obbHalfWidths, const glm::mat4 &obbOrientation) {
        // Transform sphere center to OBB's local coordinate system
        const glm::vec4 relCenter = sphereCenter - obbCenter;
        const glm::vec4 localSphereCenter = math::transposeHomogeneous(obbOrientation) * relCenter;

        // Create local AABB bounds for OBB
        const auto localAABBMin = glm::vec4(-obbHalfWidths, 0.0f);
        const auto localAABBMax = glm::vec4(obbHalfWidths, 0.0f);

        // Use sphereToAABB method to test collision in local space
        CollisionResult localResult = sphereToAABB(localSphereCenter, sphereRadius, localAABBMin, localAABBMax);

        if (localResult.collided) {
            // Transform the collision normal back to world space
            localResult.normal = obbOrientation * localResult.normal;
        }

        return localResult;
    }

    static CollisionResult sphereToAABB(const glm::vec4 &sphereCenter, float sphereRadius,
                                        const glm::vec4 &aabbMinBounds, const glm::vec4 &aabbMaxBounds) {
        CollisionResult result{};
        result.collided = false;
        result.normal = glm::vec4(0.0f);
        result.penetrationDepth = 0.0f;

        const glm::vec4 closestPoint = glm::clamp(sphereCenter, aabbMinBounds, aabbMaxBounds);

        const glm::vec4 distance = sphereCenter - closestPoint;

        const float distSquared = math::dotProduct(distance, distance);

        if (distSquared < sphereRadius * sphereRadius) {
            result.collided = true;
            const float dist = sqrt(distSquared);

            result.penetrationDepth = sphereRadius - dist;
            result.normal = math::normalize(distance);
        }

        return result;
    }

    static CollisionResult aabbToAABB(const glm::vec4 &aabb1MinBounds, const glm::vec4 &aabb1MaxBounds,
                                      const glm::vec4 &aabb2MinBounds, const glm::vec4 &aabb2MaxBounds) {
        CollisionResult result{false, glm::vec4(0.0f), 0};

        float overlapX = std::min(aabb1MaxBounds.x, aabb2MaxBounds.x) - std::max(aabb1MinBounds.x, aabb2MinBounds.x);
        float overlapY = std::min(aabb1MaxBounds.y, aabb2MaxBounds.y) - std::max(aabb1MinBounds.y, aabb2MinBounds.y);
        float overlapZ = std::min(aabb1MaxBounds.z, aabb2MaxBounds.z) - std::max(aabb1MinBounds.z, aabb2MinBounds.z);

        if (overlapX <= 0 || overlapY < 0 || overlapZ < 0) {
            return result;
        }

        result.collided = true;

        float minOverlap = std::min({overlapX, overlapY, overlapZ});
        result.penetrationDepth = minOverlap;

        if (minOverlap == overlapX) {
            result.normal = (aabb1MinBounds.x < aabb2MinBounds.x) ? glm::vec4(-1, 0, 0, 0) : glm::vec4(1, 0, 0, 0);
        } else if (minOverlap == overlapY) {
            result.normal = (aabb1MinBounds.y < aabb2MinBounds.y) ? glm::vec4(0, -1, 0, 0) : glm::vec4(0, 1, 0, 0);
        } else if (minOverlap == overlapZ) {
            result.normal = (aabb1MinBounds.z < aabb2MinBounds.z) ? glm::vec4(0, 0, -1, 0) : glm::vec4(0, 0, 1, 0);
        }

        return result;
    }

    static CollisionResult sphereToSphere(const glm::vec4 &sphere1Center, float sphere1Radius,
                                          const glm::vec4 &sphere2Center, float sphere2Radius) {
        CollisionResult result{false, glm::vec4(0.0f), 0};

        const glm::vec4 distance = sphere1Center - sphere2Center;
        const float distSquared = math::dotProduct(distance, distance);

        if (distSquared < (sphere1Radius + sphere2Radius) * (sphere1Radius + sphere2Radius)) {
            result.collided = true;
            const float dist = sqrt(distSquared);

            result.penetrationDepth = sphere1Radius + sphere2Radius - dist;
            result.normal = math::normalize(distance);
        }

        return result;
    }
};
