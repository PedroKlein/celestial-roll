#pragma once

#include "matrixUtils.hpp"
#include "mesh.hpp"

class GameObject
{
  public:
    GameObject(const Mesh &mesh) : mesh(mesh), position(0.0f), rotation(0.0f), scale(1.0f)
    {
    }

    GameObject(const Mesh &mesh, const glm::vec3 &position)
        : mesh(mesh), position(position, 1.0f), rotation(0.0f), scale(1.0f)
    {
    }

    GameObject(const Mesh &mesh, const glm::vec3 &position, const glm::vec3 &scale)
        : mesh(mesh), position(position, 1.0f), scale(scale, 1.0f), rotation(0.0f)
    {
    }

    GameObject(const Mesh &mesh, const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
        : mesh(mesh), position(position, 1.0f), rotation(rotation, 1.0f), scale(scale, 1.0f)
    {
    }

    glm::mat4 getModelMatrix() const
    {
        glm::mat4 model = MatrixUtils::identityMatrix();
        model *= MatrixUtils::translateMatrix(position.x, position.y, position.z);

        model *= MatrixUtils::rotateXMatrix(glm::radians(rotation.x));
        model *= MatrixUtils::rotateYMatrix(glm::radians(rotation.y));
        model *= MatrixUtils::rotateZMatrix(glm::radians(rotation.z));
        model *= MatrixUtils::scaleMatrix(scale.x, scale.y, scale.z);
        return model;
    }

    void draw(const Shader &shader) const
    {
        shader.setMat4("model", getModelMatrix());
        mesh.draw(shader);
    }

    const glm::vec4 getPosition() const
    {
        return position;
    }

  private:
    Mesh mesh;
    glm::vec4 position;
    glm::vec4 rotation;
    glm::vec4 scale;
};