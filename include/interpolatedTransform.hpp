#pragma once

#include "transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class InterpolatedTransform
{
  public:
    const Transform *baseTransform;
    glm::vec3 interpolatedPosition;
    glm::vec3 interpolatedScale;
    glm::quat interpolatedRotation;

    InterpolatedTransform(const Transform *transform) : baseTransform(transform)
    {
    }

    glm::vec4 getPosition() const
    {
        return glm::vec4(interpolatedPosition, 1.0f);
    }

    void calculateInterpolation(float alpha)
    {
        interpolatedPosition =
            glm::mix(glm::vec3(baseTransform->previousPosition), glm::vec3(baseTransform->position), alpha);
        interpolatedScale = baseTransform->previousScale;
        interpolatedRotation = glm::slerp(baseTransform->previousRotation, baseTransform->rotation, alpha);
    }

    glm::mat4 getInterpolatedModelMatrix() const
    {
        glm::mat4 model = math::translateMatrix(interpolatedPosition.x, interpolatedPosition.y, interpolatedPosition.z);
        model *= glm::toMat4(interpolatedRotation);
        model *= math::scaleMatrix(interpolatedScale.x, interpolatedScale.y, interpolatedScale.z);
        return model;
    }
};
