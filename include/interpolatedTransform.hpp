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

    void calculateInterpolation(float alpha)
    {
        interpolatedPosition =
            glm::mix(glm::vec3(baseTransform->previousPosition), glm::vec3(baseTransform->position), alpha);
        interpolatedScale = glm::mix(baseTransform->previousScale, baseTransform->scale, alpha);
        interpolatedRotation = glm::mix(baseTransform->previousRotation, baseTransform->rotation, alpha);
    }

    glm::mat4 getInterpolatedModelMatrix() const
    {
        glm::mat4 interpolatedRotationMatrix = Transform::getRotationMatrix(interpolatedRotation);

        glm::mat4 model =
            MatrixUtils::translateMatrix(interpolatedPosition.x, interpolatedPosition.y, interpolatedPosition.z);
        model *= interpolatedRotationMatrix;
        model *= MatrixUtils::scaleMatrix(interpolatedScale.x, interpolatedScale.y, interpolatedScale.z);

        return model;
    }
};
