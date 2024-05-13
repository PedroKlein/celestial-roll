#pragma once

#include "objLoader.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <glad/glad.h>
#include <vector>

class Mesh
{
  public:
    Mesh(const std::string &filename) : loader(filename)
    {
        if (!loader.loadModel())
        {
            std::cerr << "Failed to load model" << std::endl;
            return;
        }

        indices = loader.getIndices();

        std::cout << "Loaded " << loader.getVertices().size() << " vertices" << std::endl;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, loader.getVertices().size() * sizeof(float), &loader.getVertices()[0],
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.getIndices().size() * sizeof(unsigned int),
                     &loader.getIndices()[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        if (loader.getNormals().size() > 0)
        {
            // Normal attribute
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);
        }

        if (loader.getTexCoords().size() > 0)
        {
            // Texture Coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);
        }

        glBindVertexArray(0);
    }

    void draw(const Shader &shader) const
    {
        // unsigned int diffuseNr = 1;
        // unsigned int specularNr = 1;

        // for (unsigned int i = 0; i < textures.size(); i++)
        // {
        //     glActiveTexture(GL_TEXTURE0 + i);
        //     std::string number;
        //     std::string name = textures[i].type;
        //     if (name == "texture_diffuse")
        //         number = std::to_string(diffuseNr++);
        //     else if (name == "texture_specular")
        //         number = std::to_string(specularNr++);

        //     shader.setInt(name + number, i);
        //     glBindTexture(GL_TEXTURE_2D, textures[i].ID);
        // }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // glActiveTexture(GL_TEXTURE0);
    }

  private:
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO, VBO, EBO;

    ObjLoader loader;
};
