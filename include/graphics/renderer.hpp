#pragma once

#include "game/component.hpp"
#include "game/gameObject.hpp"
#include "graphics/mesh.hpp"
#include "matrixUtils.hpp"
#include "transform.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

class Renderer : public Component
{
  public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Transform> cachedTransform;

    Renderer(const std::shared_ptr<Mesh> &mesh) : mesh(mesh)
    {
    }

    void initialize() override
    {
        // Cache the Transform component at initialization
        cachedTransform = gameObject->getComponent<Transform>();
        if (!cachedTransform)
        {
            std::cerr << "Renderer requires a Transform component." << std::endl;
        }
    }

    void update(float deltaTime) override
    {
        if (!mesh || !cachedTransform)
        {
            std::cerr << "Renderer component is not properly initialized." << std::endl;
            return;
        }

        globalShader.setMat4("model", cachedTransform->getModelMatrix());
        mesh->draw(globalShader);
    }
};