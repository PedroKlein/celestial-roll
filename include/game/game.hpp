#pragma once

#include "game/scene.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"
#include "input/inputHandler.hpp"
#include <chrono>
#include <memory>
#include <thread>

const float targetFrameTime = 1.0f / 240.0f;
const float physicsTimeStep = 1.0f / 60.0f;

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
        float frameTime = updateDeltaTime();
        inputHandler.processInput(frameTime);

        float accumulatedTime = processPhysics(frameTime);
        renderScene(accumulatedTime);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // sleepToCapFrameRate(frameTime);
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

    float updateDeltaTime()
    {
        static auto lastFrameTime = std::chrono::high_resolution_clock::now();
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;
        return frameTime;
    }

    float processPhysics(float frameTime)
    {
        static float accumulatedTime = 0.0f;
        accumulatedTime += frameTime;
        while (accumulatedTime >= physicsTimeStep)
        {
            scene.updatePhysics(physicsTimeStep);
            accumulatedTime -= physicsTimeStep;
        }

        return accumulatedTime;
    }

    void renderScene(float accumulatedTime)
    {
        float alpha = accumulatedTime / physicsTimeStep;
        scene.render(alpha, viewRatio);
    }

    void sleepToCapFrameRate(float frameTime)
    {

        // Busy-wait loop to cap the frame rate
        // while (glfwGetTime() - frameTime < targetFrameTime)
        // {
        // }

        // Or sleeping thread (this is less precise but uses less CPU)
        float sleepTime = targetFrameTime - frameTime;
        if (sleepTime > 0)
        {
            std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
        }
    }
};