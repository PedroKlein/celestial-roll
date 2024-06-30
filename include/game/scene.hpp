#pragma once

#include "collision/collisionManager.hpp"
#include "game/gameObject.hpp"
#include "game/gameState.hpp"
#include "graphics/renderManager.hpp"
#include "math/matrix.hpp"
#include "objects/camera.hpp"
#include "objects/light.hpp"
#include "objects/platform.hpp"
#include "objects/player.hpp"
#include <algorithm>
#include <memory>
#include <vector>

class Scene
{
public:
    Scene()
    {
    }

    void init()
    {
        RenderManager::initializeShaders();

        this->freeCam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
        this->playerCam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, -30.0f);

        this->player = std::make_unique<Player>(*playerCam.get());

        this->gameState = std::make_unique<GameState>(*freeCam.get(), *playerCam.get(), *player.get());

        collisionManager = std::make_unique<CollisionManager>(*player.get());
        renderManager = std::make_unique<RenderManager>(*player.get());

        addObject(std::make_shared<Light>(Transform{glm::vec3(0.0f, 10.0f, 10.0f)}, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

        addObject(std::make_shared<Platform>(Transform{glm::vec3(0.0f, -40.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f)}));

        addObject(std::make_shared<IcePlatform>(
            Transform{glm::vec3(10.0f, -60.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f), glm::vec3(-30.0f, 0.0f, 0.0f)}));

        addObject(std::make_shared<IcePlatform>(
            Transform{glm::vec3(10.0f, -60.0f, -12.0f), glm::vec3(10.0f, 1.0f, 10.0f), glm::vec3(20.0f, 0.0f, 0.0f)}));
    }

    void updatePhysics(float deltaTime)
    {
        if (gameState->getIsPaused())
        {
            return;
        }

        player->updatePhysics(deltaTime);

        for (auto &obj : objects)
        {
            obj->updatePhysics(deltaTime);
        }

        collisionManager->checkCollisions(deltaTime);
    }

    void render(float alpha, float viewRatio)
    {
        glm::mat4 viewMatrix = gameState->getIsEagleView() ? freeCam->getViewMatrix() : playerCam->getViewMatrix();
        glm::vec3 viewPos = gameState->getIsEagleView() ? freeCam->getPosition() : playerCam->getPosition();
        glm::mat4 projectionMatrix = math::perspectiveMatrix(glm::radians(80.0f), viewRatio, -0.1f, -1000.0f);

        renderManager->render(alpha, viewMatrix, projectionMatrix, viewPos);
    }

    void addObject(std::shared_ptr<GameObject> object)
    {
        std::shared_ptr<Collider> collider = object->getComponent<Collider>();
        if (collider)
        {
            collisionManager->registerObject(object);
        }

        std::shared_ptr<Renderer> renderer = object->getComponent<Renderer>();
        if (renderer)
        {
            renderManager->addRenderer(renderer);
        }

        std::shared_ptr<LightEmitter> light = object->getComponent<LightEmitter>();
        if (light)
        {
            renderManager->addLightEmitter(light);
        }

        objects.push_back(object);
    }

    const std::vector<std::shared_ptr<GameObject>> &getObjects() const
    {
        return objects;
    }

    Player *getPlayer() const
    {
        return player.get();
    }

    Camera *getFreeCam()
    {
        return freeCam.get();
    }

    Camera *getPlayerCam()
    {
        return playerCam.get();
    }

    GameState *getGameState()
    {
        return gameState.get();
    }

private:
    std::vector<std::shared_ptr<GameObject>> objects;

    std::unique_ptr<Player> player;
    std::unique_ptr<Camera> freeCam;
    std::unique_ptr<Camera> playerCam;
    std::unique_ptr<GameState> gameState;

    std::unique_ptr<CollisionManager> collisionManager;
    std::unique_ptr<RenderManager> renderManager;
};