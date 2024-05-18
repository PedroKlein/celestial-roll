#pragma once

#include "boxCollider.hpp"
#include "camera.hpp"
#include "gameObject.hpp"
#include "inputObserver.hpp"
#include "renderer.hpp"
#include "rigidBody.hpp"

class Player : public GameObject, public InputObserver
{
  public:
    Player(Camera &camera) : camera(camera)
    {
    }

    void initialize(const std::shared_ptr<Mesh> &mesh)
    {
        transform = std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
        addComponent(transform);

        camera.setTarget(*transform, 5.0f);

        addComponent(std::make_shared<Renderer>(mesh));

        boxCollider = std::make_shared<BoxCollider>(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.5f, 2.0f, 0.5f));
        addComponent(boxCollider);

        rigidBody = std::make_shared<RigidBody>(20.0f);
        addComponent(rigidBody);
    }

    void processKeyboard(Action action, float deltaTime) override
    {
        if (!inputEnabled)
        {
            return;
        }

        // moves the player only in the xz plane
        glm::vec3 cameraFrontInXZ = glm::vec3(camera.getFront().x, 0.0f, camera.getFront().z);
        glm::vec3 cameraRightInXZ = glm::vec3(camera.getRight().x, 0.0f, camera.getRight().z);

        cameraFrontInXZ = glm::normalize(cameraFrontInXZ);
        cameraRightInXZ = glm::normalize(cameraRightInXZ);

        if (action == FORWARD)
        {
            transform->position += cameraFrontInXZ * deltaTime * movementSpeed;
        }
        if (action == BACKWARD)
        {
            transform->position -= cameraFrontInXZ * deltaTime * movementSpeed;
        }
        if (action == LEFT)
        {
            transform->position -= cameraRightInXZ * deltaTime * movementSpeed;
        }
        if (action == RIGHT)
        {
            transform->position += cameraRightInXZ * deltaTime * movementSpeed;
        }
    }

    glm::vec4 getPosition() const
    {
        return transform->getPositionHom();
    }

    void setPosition(const glm::vec3 &position)
    {
        transform->position = glm::vec4(position, 1.0f);
    }

    // get collider
    std::shared_ptr<BoxCollider> getCollider() const
    {
        return boxCollider;
    }

    // void update(float deltaTime, std::vector<std::shared_ptr<GameObject>> scene)
    // {
    //     for (auto &obj : scene)
    //     {
    //         if (this->checkCollision(*obj))
    //         {
    //             this->handleCollision(*obj);
    //             return;
    //         }
    //     }

    //     PhysicsObject::update(deltaTime);

    //     camera.updateCameraVectors();
    // }

    // void handleCollision(GameObject &other)
    // {
    //     if (typeid(other) == typeid(GameObject))
    //     {
    //         glm::vec4 normal = glm::vec4(other.getNormal(), 0.0f);

    //         glm::vec4 projectedVelocity = MatrixUtils::dotProduct(velocity, normal) * normal;

    //         velocity -= 2.0f * projectedVelocity;
    //     }
    // }

  private:
    Camera &camera;
    std::shared_ptr<Transform> transform;
    std::shared_ptr<RigidBody> rigidBody;
    std::shared_ptr<BoxCollider> boxCollider;

    float movementSpeed = 5.0f;
};