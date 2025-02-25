#pragma once

#include <cstddef>
#include <glad/glad.h>
#include <vector>
#include "objLoader.hpp"

class Mesh {
public:
    explicit Mesh(const std::string &filename) : loader(filename) {
        if (!loader.loadModel()) {
            throw std::runtime_error("Failed to load model");
        }

        indices = loader.getIndices();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, loader.getVertexAttributes().size() * sizeof(VertexAttribute),
                     &loader.getVertexAttributes()[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.getIndices().size() * sizeof(unsigned int),
                     &loader.getIndices()[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute),
                              reinterpret_cast<void *>(offsetof(VertexAttribute, position)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute),
                              reinterpret_cast<void *>(offsetof(VertexAttribute, normal)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute),
                              reinterpret_cast<void *>(offsetof(VertexAttribute, texcoord)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void draw(const Material &material) const {
        material.applyShaderProperties();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    std::vector<unsigned int> indices;
    unsigned int VAO{}, VBO{}, EBO{};

    ObjLoader loader;
};
