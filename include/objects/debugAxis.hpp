#pragma once

#include "graphics/shader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

constexpr float axisVertices[] = {
    // X axis (Red)
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    // Y axis (Green)
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    // Z axis (Blue)
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

class DebugAxis
{
  public:
    DebugAxis() : shader()
    {
        shader.initialize("resources/shaders/debug.vert", "resources/shaders/debug.frag");

        glGenVertexArrays(1, &axisVAO);
        glGenBuffers(1, &axisVBO);

        glBindVertexArray(axisVAO);
        glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void render(const glm::vec4 &position)
    {
        shader.use();
        glBindVertexArray(axisVAO);

        glm::mat4 model = math::translateMatrix(position.x, position.y, position.z);
        model *= math::scaleMatrix(2.5f, 2.5f, 2.5f);
        shader.setMat4("model", model);

        glDrawArrays(GL_LINES, 0, 6);

        glBindVertexArray(0);
    }

  private:
    GLuint axisVAO, axisVBO;
    Shader shader;
};