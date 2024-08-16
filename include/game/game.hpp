#pragma once

#include <chrono>
#include <thread>
#include "game/scene.hpp"
#include "input/inputHandler.hpp"

constexpr float targetFrameTime = 1.0f / 240.0f;
constexpr float physicsTimeStep = 1.0f / 60.0f;

class Game {
public:
    Game(const float initialWidth, const float initialHeight) {
        viewRatio = initialWidth / initialHeight;

        scene.init();

        inputHandler.addObserver(scene.getPlayer());
        inputHandler.addObserver(scene.getFreeCam());
        inputHandler.addObserver(scene.getPlayerCam());
        inputHandler.addObserver(scene.getGameState());
    }

    void tick() {
        const float frameTime = updateDeltaTime();
        inputHandler.processInput(frameTime);

        const float accumulatedTime = processPhysics(frameTime);
        renderScene(accumulatedTime);

        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // sleepToCapFrameRate(frameTime);
    }

    InputHandler *getInputHandler() { return &inputHandler; }

    void setViewRatio(const int width, const int height) {
        viewRatio = static_cast<float>(width) / static_cast<float>(height);
    }

private:
    Scene scene;
    InputHandler inputHandler;

    float viewRatio;

    static float updateDeltaTime() {
        static auto lastFrameTime = std::chrono::high_resolution_clock::now();
        const auto currentFrameTime = std::chrono::high_resolution_clock::now();
        const float frameTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;
        // std::cout << "fps: " << 1.0f / frameTime << std::endl;
        return frameTime;
    }

    float processPhysics(const float frameTime) {
        static float accumulatedTime = 0.0f;
        accumulatedTime += frameTime;
        while (accumulatedTime >= physicsTimeStep) {
            scene.updatePhysics(physicsTimeStep);
            accumulatedTime -= physicsTimeStep;
        }

        return accumulatedTime;
    }

    void renderScene(const float accumulatedTime) {
        const float alpha = accumulatedTime / physicsTimeStep;
        scene.render(alpha, viewRatio);
    }

    static void sleepToCapFrameRate(float frameTime) {

        // Busy-wait loop to cap the frame rate
        // while (glfwGetTime() - frameTime < targetFrameTime)
        // {
        // }

        // Or sleeping thread (this is less precise but uses less CPU)
        if (const float sleepTime = targetFrameTime - frameTime; sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
        }
    }
};
