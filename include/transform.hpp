#pragma once

#include "component.hpp"
#include "matrixUtils.hpp"
#include <glm/glm.hpp>
#include <iostream>

class Transform : public Component
{
  public:
    glm::vec4 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    Transform() : position(0.0f, 0.0f, 0.0f, 1.0f), rotation(0.0f), scale(1.0f)
    {
    }

    Transform(const glm::vec3 &position) : position(position, 1.0f), rotation(0.0f), scale(1.0f)
    {
    }

    Transform(const glm::vec3 &position, const glm::vec3 &scale)
        : position(position, 1.0f), scale(scale), rotation(0.0f)
    {
    }

    Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
        : position(position, 1.0f), rotation(rotation), scale(scale)
    {
    }

    glm::vec4 getPosition() const
    {
        return position;
    }

    void setPosition(const glm::vec3 &position)
    {
        this->position = glm::vec4(position, 1.0f);
    }

    glm::vec3 getScale() const
    {
        return scale;
    }

    void setScale(const glm::vec3 &scale)
    {
        this->scale = scale;
    }

    glm::vec3 getRotation() const
    {
        return rotation;
    }

    void setRotation(const glm::vec3 &rotation)
    {
        this->rotation = rotation;
        rotationMatrixDirty = true;
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

    friend std::ostream &operator<<(std::ostream &os, const Transform &transform)
    {
        os << "Position: " << transform.position.x << ", " << transform.position.y << ", " << transform.position.z
           << std::endl;
        os << "Rotation: " << transform.rotation.x << ", " << transform.rotation.y << ", " << transform.rotation.z
           << std::endl;
        os << "Scale: " << transform.scale.x << ", " << transform.scale.y << ", " << transform.scale.z << std::endl;
        return os;
    }

  private:
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