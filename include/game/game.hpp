#pragma once

#include "game/scene.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"
#include "input/inputHandler.hpp"
#include <chrono>
#include <memory>
#include <thread>

const float targetFrameTime = 1000.0f / 240.0f;
const float physicsTimeStep = 1000.0f / 60.0f;

class Game
{
  public:
    Game(float initialWidth, float initialHeight) : inputHandler(), scene()
    {
        viewRatio = initialWidth / initialHeight;

        scene.init();

        inputHandler.addObserver(scene.getPlayer());
        inputHandler.addObserver(scene.getFreeCam());
        inputHandler.addObserver(scene.getPlayerCam());
        inputHandler.addObserver(scene.getGameState());
    }

    void tick()
    {
        static auto lastFrameTime = std::chrono::high_resolution_clock::now();
        static float accumulatedTime = 0.0f;

        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();

        accumulatedTime += frameTime;

        inputHandler.processInput(frameTime / 1000.0f);

        // Update physics in fixed timesteps
        while (accumulatedTime >= physicsTimeStep)
        {
            scene.updatePhysics(physicsTimeStep / 1000.0f);
            accumulatedTime -= physicsTimeStep;
        }

        float alpha = accumulatedTime / physicsTimeStep;
        scene.render(alpha, viewRatio);

        // Sleep to cap the frame rate
        auto postUpdateTime = std::chrono::high_resolution_clock::now();
        auto sleepTime =
            targetFrameTime -
            std::chrono::duration_cast<std::chrono::milliseconds>(postUpdateTime - currentFrameTime).count();
        if (sleepTime > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds((int)sleepTime));
        }

        lastFrameTime = currentFrameTime;
    }

    InputHandler *getInputHandler()
    {
        return &inputHandler;
    }

    void setViewRatio(float width, float height)
    {
        viewRatio = width / height;
    }

  private:
    Scene scene;
    InputHandler inputHandler;

    float viewRatio;
};