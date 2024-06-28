#pragma once

#include "camera.hpp"
#include "collision/sphereCollider.hpp"
#include "game/gameObject.hpp"
#include "graphics/materialManager.hpp"
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
        transform = std::make_shared<Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
        addComponent(transform);

        renderer =
            std::make_shared<Renderer>(MeshManager::getInstance().getMesh("resources/models/sphere.obj"),
                                       MaterialManager::getInstance().getMaterial("resources/materials/sphere.mtl"));
        addComponent(renderer);

        camera.setTarget(*renderer->interpolatedTransform, 10.0f);

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

        glm::vec4 force(0.0f);
        float inputForce = movementSpeed * 10.0f;

        if (action == FORWARD)
        {
            force += cameraFrontInXZ * inputForce;
        }
        if (action == BACKWARD)
        {
            force -= cameraFrontInXZ * inputForce;
        }
        if (action == LEFT)
        {
            force -= cameraRightInXZ * inputForce;
        }
        if (action == RIGHT)
        {
            force += cameraRightInXZ * inputForce;
        }

        if (rigidBody->isGrounded)
        {
            // TODO: make jump also in the direction of player input
            if (action == JUMP)
            {
                glm::vec4 jumpDirection = currentSurfaceNormal;
                const float jumpStrength = 10.0f;

                transform->position += currentSurfaceNormal * 0.1f;
                rigidBody->velocity = jumpDirection * jumpStrength;
            }

            glm::vec4 adjustedForce = force - currentSurfaceNormal * glm::dot(force, currentSurfaceNormal);
            force.x = adjustedForce.x;
            force.y = 0.0f;
            force.z = adjustedForce.z;
        }

        rigidBody->addInputForce(force, deltaTime);
    }

    void render(float alpha)
    {
        GameObject::render(alpha);

        auto interpolatedPosition = getInterpolatedPosition();

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

    void handleCollision(GameObject &other, const glm::vec4 collisionNormal, float penetrationDepth, float deltaTime)
    {
        auto otherTransform = other.getComponent<Transform>();
        auto physicsMat = other.getComponent<PhysicsMaterial>();

        if (!otherTransform || !physicsMat)
        {
            return;
        }

        glm::vec4 normalComponent = math::dotProduct(rigidBody->velocity, collisionNormal) * collisionNormal;
        glm::vec4 tangentialComponent = rigidBody->velocity - normalComponent;

        glm::vec4 bounceVelocity = -normalComponent * physicsMat->getBounciness();
        glm::vec4 frictionVelocity = tangentialComponent * (1.0f - physicsMat->getFriction());

        if (other.getObjectType() == ObjectType::Platform)
        {
            rigidBody->velocity = bounceVelocity + frictionVelocity;

            Debug::printVec(collisionNormal);

            if (rigidBody->isGrounded && collisionNormal.y > 0.7)
            {
                currentSurfaceNormal = collisionNormal;
                updateRotation(deltaTime);
            }
        }
        else
        {
            rigidBody->velocity -= 2.0f * normalComponent;
        }

        transform->position += collisionNormal * (penetrationDepth / 2);
    }

    void setIsGrounded(bool isGrounded)
    {
        rigidBody->isGrounded = isGrounded;
    }

    ObjectType getObjectType() const override
    {
        return ObjectType::Player;
    }

    int getShaderId() const
    {
        return renderer->material->shader->ID;
    }

    void updateRotation(float deltaTime)
    {
        glm::vec4 movementDirection = glm::normalize(rigidBody->velocity);
        glm::vec4 rotationAxis = math::crossProduct(movementDirection, Camera::getWorldUp());
        if (glm::length(rotationAxis) > 0)
        {
            float movementDistance = glm::length(glm::vec3(rigidBody->velocity)) * deltaTime;
            const float rotationSpeedFactor = 10.0f;
            float rotationAngle =
                -movementDistance / (2.0f * glm::pi<float>() * transform->scale.x) * rotationSpeedFactor;
            glm::quat rotationDelta = glm::angleAxis(rotationAngle, glm::vec3(rotationAxis));
            transform->rotation = glm::normalize(rotationDelta * transform->rotation);
        }
    }

  private:
    Camera &camera;
    glm::vec4 currentSurfaceNormal;
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<RigidBody> rigidBody;
    std::shared_ptr<SphereCollider> sphereCollider;
    std::shared_ptr<GravityComponent> gravity;

    float movementSpeed = 2.0f;

    glm::vec4 getInterpolatedPosition() const
    {
        return renderer->interpolatedTransform->getPosition();
    }
};