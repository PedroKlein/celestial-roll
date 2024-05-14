#pragma once

#include "Camera.hpp"
#include "InputObserver.hpp"
#include "Player.hpp"

class GameState : public InputObserver
{
  public:
    GameState(Camera &freeCam, Camera &playerCam, Player &player)
        : isPaused(false), isEagleView(false), freeCam(freeCam), playerCam(playerCam), player(player)
    {
    }

    void processKeyboard(Action action, float deltaTime) override
    {
        if (action == PAUSE)
        {
            isPaused = !isPaused;
            if (isPaused)
            {
                disableControls();
                return;
            }

            isEagleView ? enableEagleView() : enablePlayerView();
            return;
        }

        if (action == EAGLE_VIEW)
        {
            isEagleView = !isEagleView;
            isEagleView ? enableEagleView() : enablePlayerView();
        }
    }

    bool getIsPaused() const
    {
        return isPaused;
    }

    bool getIsEagleView() const
    {
        return isEagleView;
    }

  private:
    Camera &freeCam;
    Camera &playerCam;
    Player &player;

    bool isPaused;
    bool isEagleView;

    void disableControls()
    {
        freeCam.setInputEnabled(false);
        playerCam.setInputEnabled(false);
        player.setInputEnabled(false);
    }

    void enableEagleView()
    {
        freeCam.setInputEnabled(true);
        playerCam.setInputEnabled(false);
        player.setInputEnabled(false);
    }

    void enablePlayerView()
    {
        freeCam.setInputEnabled(false);
        playerCam.setInputEnabled(true);
        player.setInputEnabled(true);
    }
};