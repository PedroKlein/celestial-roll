#pragma once

#include <vector>
#include "collision/collisionManager.hpp"
#include "game/gameObject.hpp"
#include "game/gameState.hpp"
#include "graphics/renderManager.hpp"
#include "math/matrix.hpp"
#include "objects/camera.hpp"
#include "objects/deathBox.hpp"
#include "objects/duck.hpp"
#include "objects/light.hpp"
#include "objects/platform.hpp"
#include "objects/player.hpp"
#include "objects/star.hpp"

class Scene {
public:
    Scene() = default;

    void init() {
        RenderManager::initializeShaders();
        RenderManager::initializeMaterials();

        this->freeCam = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
        this->playerCam = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, -30.0f);

        this->player = std::make_shared<Player>(*playerCam);

        this->gameState = std::make_unique<GameState>(*freeCam, *playerCam, *player);

        collisionManager = std::make_unique<CollisionManager>(*player);
        renderManager = std::make_unique<RenderManager>(*player);

        addObject(player);
        addObject(freeCam);
        addObject(playerCam);

        // Global light
        addObject(std::make_shared<Light>(Transform{glm::vec3(100.0f, 100.0f, 100.0f)},
                                          glm::vec4(1.0f, 1.0f, 1.0f, 0.1f), 1.0f, 0, 0));

        addObject(std::make_shared<DeathBox>(
                Transform{glm::vec3(0.0f, -40.0f, 0.0f), glm::vec3(1000.0f, 1.0f, 1000.0f)}));

        addObject(std::make_shared<Duck>(Transform{glm::vec3(4.0f, -9.0f, -8.0f), glm::vec3(0.5f, 0.5f, 0.5f)}));

        addObject(std::make_shared<Star>(Transform{glm::vec3(60.0f, -10.0f, -20.0f), glm::vec3(5.0f, 5.0f, 5.0f)}));

        initializePlatforms();
    }

    void updatePhysics(const float deltaTime) const {
        if (gameState->getIsPaused()) {
            return;
        }

        for (auto &source: gravitationalSources) {
            player->addGravitationalSource(*source);
        }

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

        if (object->getObjectType() == ObjectType::Star) {
            gravitationalSources.push_back(object->getComponent<RigidBody>());
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
    std::vector<std::shared_ptr<RigidBody>> gravitationalSources;

    std::shared_ptr<Player> player;
    std::shared_ptr<Camera> freeCam;
    std::shared_ptr<Camera> playerCam;
    std::unique_ptr<GameState> gameState;

    std::unique_ptr<CollisionManager> collisionManager;
    std::unique_ptr<RenderManager> renderManager;

    void initializePlatforms() {
        addObject(std::make_shared<Platform>(Transform{glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)}));
        addObject(std::make_shared<Platform>(Transform{glm::vec3(20.0f, -10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.98f)}));
        addObject(std::make_shared<IcePlatform>(Transform{glm::vec3(38.18f, -14.86f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                                          glm::vec3(0.0f, 0.0f, -30.0f)}));
        addObject(std::make_shared<IcePlatform>(
                Transform{glm::vec3(56.33f, -20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.98f), glm::vec3(0.0f, 0.0f, 0.0f)}));
        addObject(std::make_shared<IcePlatform>(
                Transform{glm::vec3(74.0f, -15.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 30.0f)}));
        addObject(std::make_shared<Platform>(Transform{glm::vec3(60.0f, -20.0f, 15.0f), glm::vec3(1.0f, 1.0f, 0.98f),
                                                       glm::vec3(0.0f, 90.0f, 0.0f)}));
        addObject(std::make_shared<Platform>(Transform{glm::vec3(60.0f, -17.0f, 33.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                                       glm::vec3(90.0f, -70.0f, 90.0f)}));
        addObject(std::make_shared<JumpPlatform>(Transform{
                glm::vec3(60.0f, -13.66f, 52.1f), glm::vec3(1.0f, 1.0f, 0.98f), glm::vec3(0.0f, 90.0f, 00.0f)}));

        auto movingPlatform = std::make_shared<JumpPlatform>(
                Transform{glm::vec3(80.0f, -12.0f, 55.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)});
        std::vector<glm::vec3> points = {
                glm::vec3(0.0f, 0.0f, 0.0f), // P0
                glm::vec3(0.0f, 0.0f, 19.40f), // P1
                glm::vec3(100.0f, 0.0f, 11.10f), // P2
                glm::vec3(100.0f, 0.0f, 0.0f) // P3
        };
        movingPlatform->addComponent(std::make_shared<BezierAnimation>(points, 20.0f));
        addObject(movingPlatform);

        addObject(std::make_shared<Platform>(
                Transform{glm::vec3(200.0f, -12.0f, 55.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)}));
    }
};
