#pragma once

#include "mesh.hpp"
#include "shader.hpp"
#include <vector>

class Renderer
{
  public:
    Renderer()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void RenderScene(const std::vector<Mesh> &meshes, Shader &shader, const glm::mat4 &viewMatrix,
                     const glm::mat4 &projectionMatrix)
    {
        shader.use();
        shader.setMat4("view", viewMatrix);
        shader.setMat4("projection", projectionMatrix);

        for (auto &mesh : meshes)
        {
            mesh.Draw(shader);
        }
    }

    void Clear()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};
