#pragma once

#include "matrixUtils.hpp"
#include <glm/glm.hpp>

class Transform
{
  public:
    glm::vec3 position;
    glm::vec3 scale;

    Transform() : position(0.0f), rotation(0.0f), scale(1.0f)
    {
    }

    Transform(const glm::vec3 &position) : position(position), rotation(0.0f), scale(1.0f)
    {
    }

    Transform(const glm::vec3 &position, const glm::vec3 &scale) : position(position), scale(scale), rotation(0.0f)
    {
    }

    Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }

    glm::mat4 getModelMatrix() const
    {
        updateRotationMatrix();
        glm::mat4 model = MatrixUtils::translateMatrix(position.x, position.y, position.z);
        model *= rotationMatrix;
        model *= MatrixUtils::scaleMatrix(scale.x, scale.y, scale.z);
        return model;
    }

    glm::vec3 getNormal() const
    {
        updateRotationMatrix();
        glm::vec3 normal = glm::vec3(rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2]);
        return glm::normalize(normal);
    }

  private:
    glm::vec3 rotation;
    mutable glm::mat4 rotationMatrix;
    mutable bool rotationMatrixDirty = true;

    void updateRotationMatrix() const
    {
        if (rotationMatrixDirty)
        {
            rotationMatrix = MatrixUtils::rotateXMatrix(glm::radians(rotation.x));
            rotationMatrix *= MatrixUtils::rotateYMatrix(glm::radians(rotation.y));
            rotationMatrix *= MatrixUtils::rotateZMatrix(glm::radians(rotation.z));
            rotationMatrixDirty = false;
        }
    }
};
;