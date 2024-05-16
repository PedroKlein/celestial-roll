#pragma once

#include "gameObject.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include <memory>
#include <vector>

class Renderer
{
  public:
    Renderer()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void renderScene(const Scene &scene, Shader &shader)
    {
        clear();

        shader.use();
        shader.setMat4("view", scene.getViewMatrix());
        shader.setMat4("projection", scene.getProjectionMatrix());

        for (auto &objects : scene.getObjects())
        {
            objects->draw(shader);
        }

        scene.getPlayer()->draw(shader);
    }

  private:
    void clear()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};
