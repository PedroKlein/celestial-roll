#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"

class Skybox {
public:
    Skybox() : shader() {
        shader.initialize("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
        setupSkybox();
        const std::vector<std::string> faces = {"resources/textures/skybox/px.png", "resources/textures/skybox/nx.png",
                                                "resources/textures/skybox/py.png", "resources/textures/skybox/ny.png",
                                                "resources/textures/skybox/pz.png", "resources/textures/skybox/nz.png"};
        cubeMapTexture = std::make_unique<Texture>(faces);
        rotationMatrix = math::rotateYMatrix(glm::radians(-150.0f));
    }

    void render() {
        shader.use();
        shader.setMat4("rotation", rotationMatrix);
        glDepthFunc(GL_LEQUAL);
        cubeMapTexture->bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    ~Skybox() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

private:
    GLuint VAO, VBO;
    Shader shader;
    std::unique_ptr<Texture> cubeMapTexture;
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    void setupSkybox() {
        float skyboxVertices[] = {
                // Positions
                // Back face
                -1.0f, -1.0f, -1.0f, // Bottom-left
                1.0f, 1.0f, -1.0f, // top-right
                1.0f, -1.0f, -1.0f, // bottom-right
                1.0f, 1.0f, -1.0f, // top-right
                -1.0f, -1.0f, -1.0f, // bottom-left
                -1.0f, 1.0f, -1.0f, // top-left
                // Front face
                -1.0f, -1.0f, 1.0f, // bottom-left
                1.0f, -1.0f, 1.0f, // bottom-right
                1.0f, 1.0f, 1.0f, // top-right
                1.0f, 1.0f, 1.0f, // top-right
                -1.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, 1.0f, // bottom-left
                // Left face
                -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, 1.0f, -1.0f, // top-left
                -1.0f, -1.0f, -1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, // bottom-left
                -1.0f, -1.0f, 1.0f, // bottom-right
                -1.0f, 1.0f, 1.0f, // top-right
                // Right face
                1.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f, -1.0f, // bottom-right
                1.0f, 1.0f, -1.0f, // top-right
                1.0f, -1.0f, -1.0f, // bottom-right
                1.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f, 1.0f, // bottom-left
                // Bottom face
                -1.0f, -1.0f, -1.0f, // top-right
                1.0f, -1.0f, -1.0f, // top-left
                1.0f, -1.0f, 1.0f, // bottom-left
                1.0f, -1.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, 1.0f, // bottom-right
                -1.0f, -1.0f, -1.0f, // top-right
                // Top face
                -1.0f, 1.0f, -1.0f, // top-left
                1.0f, 1.0f, 1.0f, // bottom-right
                1.0f, 1.0f, -1.0f, // top-right
                1.0f, 1.0f, 1.0f, // bottom-right
                -1.0f, 1.0f, -1.0f, // top-left
                -1.0f, 1.0f, 1.0f // bottom-left
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glBindVertexArray(0);
    }
};
