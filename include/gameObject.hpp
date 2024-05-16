#pragma once

#include "boxCollider.hpp"
#include "collider.hpp"
#include "matrixUtils.hpp"
#include "mesh.hpp"
#include "sphereCollider.hpp"
#include "transform.hpp"
#include <memory>

class GameObject
{
  public:
    GameObject(const Mesh &mesh) : mesh(mesh), transform()
    {
    }

    GameObject(const Mesh &mesh, const glm::vec3 &position) : mesh(mesh), transform(position)
    {
    }

    GameObject(const Mesh &mesh, const glm::vec3 &position, const glm::vec3 &scale)
        : mesh(mesh), transform(position, scale)
    {
    }

    GameObject(const Mesh &mesh, const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
        : mesh(mesh), transform(position, rotation, scale)
    {
    }

    GameObject(const GameObject &other) : mesh(other.mesh), transform(other.transform)
    {
        if (other.collider)
        {
            collider = other.collider->clone();
        }
    }

    void draw(const Shader &shader) const
    {
        shader.setMat4("model", transform.getModelMatrix());
        mesh.draw(shader);
    }

    const glm::vec4 getPosition() const
    {
        return glm::vec4(transform.position, 1.0f);
    }

    bool checkCollision(const GameObject &other) const
    {
        if (collider && other.getCollider())
        {
            return collider->checkCollision(*other.getCollider());
        }
        return false;
    }

    // create Box Collider
    void createBoxCollider(const glm::vec3 &minBounds, const glm::vec3 &maxBounds)
    {
        collider = std::make_unique<BoxCollider>(transform, minBounds, maxBounds);
    }

    // crate Sphere Collider
    void createSphereCollider(float radius)
    {
        collider = std::make_unique<SphereCollider>(transform, radius);
    }

    Collider *getCollider() const
    {
        return collider.get();
    }

  protected:
    Transform transform;
    Mesh mesh;

  private:
    std::unique_ptr<Collider> collider;
};