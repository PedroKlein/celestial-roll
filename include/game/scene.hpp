#pragma once

#include <vector>
#include "collision/collisionManager.hpp"
#include "game/gameObject.hpp"
#include "game/gameState.hpp"
#include "graphics/renderManager.hpp"
#include "math/matrix.hpp"
#include "objects/camera.hpp"
#include "objects/light.hpp"
#include "objects/platform.hpp"
#include "objects/player.hpp"

class Scene {
public:
    Scene() = default;

    void init() {
        RenderManager::initializeShaders();

        this->freeCam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
        this->playerCam = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, -30.0f);

        this->player = std::make_unique<Player>(*playerCam);

        this->gameState = std::make_unique<GameState>(*freeCam, *playerCam, *player);

        collisionManager = std::make_unique<CollisionManager>(*player);
        renderManager = std::make_unique<RenderManager>(*player);

        addObject(std::make_shared<Light>(Transform{glm::vec3(0.0f, 10.0f, 10.0f)}, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

        addObject(std::make_shared<Platform>(Transform{glm::vec3(0.0f, -40.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f)}));

        addObject(std::make_shared<IcePlatform>(Transform{glm::vec3(10.0f, -60.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f),
                                                          glm::vec3(-30.0f, 0.0f, 0.0f)}));

        addObject(std::make_shared<IcePlatform>(Transform{
                glm::vec3(10.0f, -60.0f, -12.0f), glm::vec3(10.0f, 1.0f, 10.0f), glm::vec3(20.0f, 0.0f, 0.0f)}));
    }

    void updatePhysics(const float deltaTime) const {
        if (gameState->getIsPaused()) {
            return;
        }

        player->updatePhysics(deltaTime);

        for (auto &obj: objects) {
            obj->updatePhysics(deltaTime);
        }

        collisionManager->checkCollisions(deltaTime);
    }

    void render(const float alpha, const float viewRatio) const {
        const glm::mat4 viewMatrix =
                gameState->getIsEagleView() ? freeCam->getViewMatrix() : playerCam->getViewMatrix();
        const glm::vec3 viewPos = gameState->getIsEagleView() ? freeCam->getPosition() : playerCam->getPosition();
        const glm::mat4 projectionMatrix = math::perspectiveMatrix(glm::radians(80.0f), viewRatio, -0.1f, -1000.0f);

        renderManager->render(alpha, viewMatrix, projectionMatrix, viewPos);
    }

    void addObject(const std::shared_ptr<GameObject> &object) {
        if (const std::shared_ptr<Collider> collider = object->getComponent<Collider>()) {
            collisionManager->registerObject(object);
        }

        if (const std::shared_ptr<Renderer> renderer = object->getComponent<Renderer>()) {
            renderManager->addRenderer(renderer);
        }

        if (const std::shared_ptr<LightEmitter> light = object->getComponent<LightEmitter>()) {
            renderManager->addLightEmitter(light);
        }

        objects.push_back(object);
    }

    [[nodiscard]] const std::vector<std::shared_ptr<GameObject>> &getObjects() const { return objects; }

    [[nodiscard]] Player *getPlayer() const { return player.get(); }

    [[nodiscard]] Camera *getFreeCam() const { return freeCam.get(); }

    [[nodiscard]] Camera *getPlayerCam() const { return playerCam.get(); }

    [[nodiscard]] GameState *getGameState() const { return gameState.get(); }

private:
    std::vector<std::shared_ptr<GameObject>> objects;

    std::unique_ptr<Player> player;
    std::unique_ptr<Camera> freeCam;
    std::unique_ptr<Camera> playerCam;
    std::unique_ptr<GameState> gameState;

    std::unique_ptr<CollisionManager> collisionManager;
    std::unique_ptr<RenderManager> renderManager;
};
