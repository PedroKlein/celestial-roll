#pragma once

#include "matrixUtils.hpp"
#include <glm/glm.hpp>

class Transform
{
  public:
    glm::vec3 position;
    glm::vec3 rotation;
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
        glm::mat4 model = MatrixUtils::translateMatrix(position.x, position.y, position.z);
        model *= MatrixUtils::rotateXMatrix(glm::radians(rotation.x));
        model *= MatrixUtils::rotateYMatrix(glm::radians(rotation.y));
        model *= MatrixUtils::rotateZMatrix(glm::radians(rotation.z));
        model *= MatrixUtils::scaleMatrix(scale.x, scale.y, scale.z);
        return model;
    }
};