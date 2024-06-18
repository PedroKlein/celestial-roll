#pragma once

#include "transform.hpp"
#include <glm/glm.hpp>

class InterpolatedTransform
{
  public:
    const Transform *baseTransform;
    glm::vec3 interpolatedPosition;
    glm::vec3 interpolatedScale;
    glm::vec3 interpolatedRotation;

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
        // not doing interpolation for scale and rotation for now
        interpolatedScale = baseTransform->previousScale;
        interpolatedRotation = baseTransform->previousRotation;
    }

    glm::mat4 getInterpolatedModelMatrix() const
    {
        glm::mat4 model =
            MatrixUtils::translateMatrix(interpolatedPosition.x, interpolatedPosition.y, interpolatedPosition.z);
        // TODO: investigate why for collisions the rotation on xz is mirrored from the render, the minus sign is a hack
        model *= Transform::getRotationMatrix(-interpolatedRotation);
        model *= MatrixUtils::scaleMatrix(interpolatedScale.x, interpolatedScale.y, interpolatedScale.z);
        return model;
    }
};
