#pragma once

#include <glad/glad.h>
#include <string>

#include <iostream>
#include "stb_image.h"

class Texture {
public:
    Texture(const char *texturePath, std::string typeName, const GLint wrapMode = GL_REPEAT,
            const GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, const GLint magFilter = GL_LINEAR) :
        type(std::move(typeName)), path(texturePath) {
        glGenTextures(1, &ID);
        if (ID == 0)
            throw std::runtime_error("Failed to generate texture");

        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = 0;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            throw std::runtime_error("Failed to load texture at path: " + std::string(texturePath));
        }

        stbi_image_free(data);
    }

    [[nodiscard]] unsigned int getID() const { return ID; }

    [[nodiscard]] std::string getType() const { return type; }

    [[nodiscard]] std::string getPath() const { return path; }

    ~Texture() {
        if (ID != 0)
            glDeleteTextures(1, &ID);
    }

private:
    unsigned int ID{};
    std::string type;
    std::string path;
};
