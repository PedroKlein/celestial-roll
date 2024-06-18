#pragma once

#include "debug.hpp"
#include "game/component.hpp"
#include "matrixUtils.hpp"
#include <glm/glm.hpp>
#include <iostream>

class Transform : public Component
{
  public:
    glm::vec4 position, previousPosition;
    glm::vec3 scale, previousScale;
    glm::vec3 rotation, previousRotation;

    Transform() : position(0.0f, 0.0f, 0.0f, 1.0f), rotation(0.0f), scale(1.0f)
    {
        saveState();
    }

    Transform(const glm::vec3 &position) : position(position, 1.0f), rotation(0.0f), scale(1.0f)
    {
        saveState();
    }

    Transform(const glm::vec3 &position, const glm::vec3 &scale)
        : position(position, 1.0f), scale(scale), rotation(0.0f)
    {
        saveState();
    }

    Transform(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation)
        : position(position, 1.0f), rotation(rotation), scale(scale)
    {
        this->rotation = glm::radians(rotation);
        saveState();
        updateRotationMatrix();
    }

    void saveState()
    {
        previousPosition = position;
        previousScale = scale;
        previousRotation = rotation;
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
        glm::mat4 model = MatrixUtils::translateMatrix(position.x, position.y, position.z);
        model *= getRotationMatrix();
        model *= MatrixUtils::scaleMatrix(scale.x, scale.y, scale.z);
        return model;
    }

    glm::vec3 getUp() const
    {
        updateRotationMatrix();
        glm::vec3 up = glm::vec3(rotationMatrix[0][1], rotationMatrix[1][1], rotationMatrix[2][1]);
        return glm::normalize(up);
    }

    glm::mat4 getRotationMatrix() const
    {
        updateRotationMatrix();
        return rotationMatrix;
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

    static glm::mat4 getRotationMatrix(const glm::vec3 &rotation)
    {
        glm::mat4 rotationMatrix = glm::mat4(1.0f);

        rotationMatrix = MatrixUtils::rotateXMatrix(rotation.x);
        rotationMatrix *= MatrixUtils::rotateYMatrix(rotation.y);
        rotationMatrix *= MatrixUtils::rotateZMatrix(rotation.z);

        return rotationMatrix;
    }

  private:
    mutable glm::mat4 rotationMatrix;
    mutable bool rotationMatrixDirty = true;

    void updateRotationMatrix() const
    {
        if (rotationMatrixDirty)
        {
            rotationMatrix = getRotationMatrix(rotation);
            rotationMatrixDirty = false;
        }
    }
};
;