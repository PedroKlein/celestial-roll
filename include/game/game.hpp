#pragma once

#include "game/scene.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"
#include "input/inputHandler.hpp"
#include <memory>

class Game
{
  public:
    Game(float initialWidth, float initialHeight) : deltaTime(0.0f), lastFrame(0.0f), inputHandler(), scene()
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
        updateDeltaTime();
        inputHandler.processInput(deltaTime);

        scene.update(deltaTime, viewRatio);
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

    float deltaTime;
    float lastFrame;

    void updateDeltaTime()
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
};