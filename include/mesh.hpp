#ifndef MESH_H
#define MESH_H

#include "objLoader.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <glad/glad.h>
#include <vector>

class Mesh
{
  public:
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO, VBO, EBO;

    Mesh(const ObjLoader &loader)
    {
        indices = loader.GetIndices();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, loader.GetVertices().size() * sizeof(float), &loader.GetVertices()[0],
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.GetIndices().size() * sizeof(unsigned int),
                     &loader.GetIndices()[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        if (loader.GetNormals().size() > 0)
        {
            // Normal attribute
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);
        }

        if (loader.GetTexCoords().size() > 0)
        {
            // Texture Coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);
        }

        glBindVertexArray(0);
    }

    void Draw(Shader &shader) const
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

        glm::mat4 model = MatrixUtils::identityMatrix();
        shader.setMat4("model", model);

        glBindVertexArray(0);

        // glActiveTexture(GL_TEXTURE0);
    }
};

#endif
