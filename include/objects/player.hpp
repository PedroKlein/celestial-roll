#pragma once

#include "camera.hpp"
#include "collision/sphereCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/mesh.hpp"
#include "graphics/meshManager.hpp"
#include "graphics/renderer.hpp"
#include "input/inputObserver.hpp"
#include "physics/gravityComponent.hpp"
#include "physics/physicsMaterial.hpp"
#include "physics/rigidBody.hpp"

class Player : public GameObject, public InputObserver
{
  public:
    Player(Camera &camera) : camera(camera)
    {
        transform =
            std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f, -90.0f, 0.0f));
        addComponent(transform);

        camera.setTarget(*transform, 10.0f);

        addComponent(std::make_shared<Renderer>(MeshManager::getInstance().getMesh("resources/models/sphere.obj")));

        sphereCollider = std::make_shared<SphereCollider>(1.0f);
        addComponent(sphereCollider);

        rigidBody = std::make_shared<RigidBody>(20.0f);
        addComponent(rigidBody);

        gravity = std::make_shared<GravityComponent>();
        gravity->disable();
        addComponent(gravity);
    }

    void processKeyboard(Action action, float deltaTime) override
    {
        if (!inputEnabled)
        {
            return;
        }

        gravity->enable();

        // moves the player only in the xz plane
        glm::vec4 cameraFrontInXZ = glm::vec4(camera.getFront().x, 0.0f, camera.getFront().z, 0.0f);
        glm::vec4 cameraRightInXZ = glm::vec4(camera.getRight().x, 0.0f, camera.getRight().z, 0.0f);

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

    void render(float deltaTime)
    {
        GameObject::render(deltaTime);

        camera.updateCameraVectors();
    }

    glm::vec4 getPosition() const
    {
        return transform->getPosition();
    }

    void setPosition(const glm::vec3 &position)
    {
        transform->position = glm::vec4(position, 1.0f);
    }

    std::shared_ptr<SphereCollider> getCollider() const
    {
        return sphereCollider;
    }

    void handleCollision(GameObject &other, const glm::vec4 collisionNormal, float penetrationDepth)
    {
        const static float epsilon = 0.01f;
        std::shared_ptr<Transform> otherTransform = other.getComponent<Transform>();
        std::shared_ptr<PhysicsMaterial> physicsMat = other.getComponent<PhysicsMaterial>();

        glm::vec4 normalComponentOfVelocity =
            MatrixUtils::dotProduct(rigidBody->velocity, collisionNormal) * collisionNormal;

        glm::vec4 tangentialComponent = rigidBody->velocity - normalComponentOfVelocity;

        float bounciness = (physicsMat) ? physicsMat->getBounciness() : 0.0f;
        glm::vec4 bounceVelocity = -normalComponentOfVelocity * bounciness;

        float friction = (physicsMat) ? physicsMat->getFriction() : 0.0f;
        glm::vec4 frictionVelocity = tangentialComponent * (1.0f - friction);

        switch (other.getObjectType())
        {
        case ObjectType::Platform:
            if (physicsMat)
            {
                rigidBody->velocity = bounceVelocity + frictionVelocity;

                if (collisionNormal.y > epsilon)
                {
                    rigidBody->isGrounded = true;
                }
            }
            else
            {
                rigidBody->velocity -= 2.0f * normalComponentOfVelocity;
            }
            break;
        default:
            rigidBody->velocity -= 2.0f * normalComponentOfVelocity;
            break;
        }

        // Move the player out of the platform
        transform->position += collisionNormal * penetrationDepth;
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Player;
    }

  private:
    Camera &camera;
    std::shared_ptr<Transform> transform;
    std::shared_ptr<RigidBody> rigidBody;
    std::shared_ptr<SphereCollider> sphereCollider;
    std::shared_ptr<GravityComponent> gravity;

    float movementSpeed = 5.0f;
};