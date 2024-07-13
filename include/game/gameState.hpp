#pragma once

#include "input/inputObserver.hpp"
#include "objects/camera.hpp"
#include "objects/player.hpp"

class GameState final : public InputObserver {
public:
    GameState(Camera &freeCam, Camera &playerCam, Player &player) :
        freeCam(freeCam), playerCam(playerCam), player(player), isPaused(false), isEagleView(false) {}

    void processKeyboard(const Action action, const float deltaTime) override {
        if (action == PAUSE) {
            isPaused = !isPaused;
            if (isPaused) {
                disableControls();
                return;
            }

            isEagleView ? enableEagleView() : enablePlayerView();
            return;
        }

        if (action == EAGLE_VIEW) {
            isEagleView = !isEagleView;
            isEagleView ? enableEagleView() : enablePlayerView();
        }
    }

    [[nodiscard]] bool getIsPaused() const { return isPaused; }

    [[nodiscard]] bool getIsEagleView() const { return isEagleView; }

private:
    Camera &freeCam;
    Camera &playerCam;
    Player &player;

    bool isPaused;
    bool isEagleView;

    void disableControls() const {
        freeCam.setInputEnabled(false);
        playerCam.setInputEnabled(false);
        player.setInputEnabled(false);
    }

    void enableEagleView() const {
        freeCam.setInputEnabled(true);
        playerCam.setInputEnabled(false);
        player.setInputEnabled(false);
    }

    void enablePlayerView() const {
        freeCam.setInputEnabled(false);
        playerCam.setInputEnabled(true);
        player.setInputEnabled(true);
    }
};
