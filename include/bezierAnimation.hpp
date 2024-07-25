#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "transform.hpp"

class BezierAnimation final : public Component {
public:
    BezierAnimation(const std::vector<glm::vec3> &points, float duration, bool loop = true) :
        controlPoints(points), totalTime(duration), currentTime(0.0f), isLooping(loop), isForward(true) {}

    void initialize() override {
        transform = gameObject->getComponent<Transform>();
        if (!transform) {
            std::cerr << "BezierAnimation requires a Transform component.\n";
            return;
        }
        initialPosition = transform->getPosition();
    }

    void update(float deltaTime) override {
        if (isLooping) {
            currentTime += (isForward ? 1 : -1) * deltaTime;
            if (currentTime > totalTime) {
                isForward = false;
                currentTime = totalTime;
            } else if (currentTime < 0) {
                isForward = true;
                currentTime = 0;
            }
        }

        float t = currentTime / totalTime;
        glm::vec3 position = calculateBezierPoint(t);
        transform->setPosition(initialPosition + position);
    }

private:
    std::shared_ptr<Transform> transform;
    glm::vec3 initialPosition;
    std::vector<glm::vec3> controlPoints;
    float totalTime;
    float currentTime;
    bool isLooping;
    bool isForward;

    glm::vec3 calculateBezierPoint(float t) {
        glm::vec3 p0 = controlPoints[0];
        glm::vec3 p1 = controlPoints[1];
        glm::vec3 p2 = controlPoints[2];
        glm::vec3 p3 = controlPoints[3];
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;
        glm::vec3 p = uuu * p0;
        p += 3 * uu * t * p1;
        p += 3 * u * tt * p2;
        p += ttt * p3;
        return p;
    }
};
