#pragma once

#include "material.hpp"
#include <cassert>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <tiny_obj_loader.h>

class ObjLoader
{
  public:
    ObjLoader(const std::string &filename) : filename(filename)
    {
    }

    bool loadModel()
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str()))
        {
            std::cerr << "TinyObjLoader Error: " << err << std::endl;
            return false;
        }

        if (!err.empty())
        {
            std::cerr << "TinyObjLoader Warning: " << err << std::endl;
        }

        if (!materials.empty())
        {
            const auto &material = materials[0];
            this->material.ambient = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
            this->material.diffuse = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
            this->material.specular = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
            this->material.shininess = material.shininess;

            if (!material.diffuse_texname.empty())
            {
                this->material.setDiffuseTexture(material.diffuse_texname);
            }
        }

        return loadAttributes(attrib, shapes);
    }

    const std::vector<float> &getVertexAttributes() const
    {
        return vertexAttributes;
    }
    const std::vector<unsigned int> &getIndices() const
    {
        return indices;
    }

    const Material &getMaterial() const
    {
        return material;
    }

  private:
    std::string filename;
    Material material;
    std::vector<float> vertexAttributes;
    std::vector<unsigned int> indices;

    bool loadAttributes(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes)
    {
        for (const auto &shape : shapes)
        {
            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
            {
                int fv = shape.mesh.num_face_vertices[f];

                for (int v = 0; v < fv; v++)
                {
                    tinyobj::index_t idx = shape.mesh.indices[f * fv + v];

                    // Position
                    vertexAttributes.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                    vertexAttributes.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                    vertexAttributes.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                    vertexAttributes.push_back(1.0f); // Homogeneous coordinate for position

                    // Normal
                    if (idx.normal_index >= 0)
                    {
                        vertexAttributes.push_back(attrib.normals[3 * idx.normal_index + 0]);
                        vertexAttributes.push_back(attrib.normals[3 * idx.normal_index + 1]);
                        vertexAttributes.push_back(attrib.normals[3 * idx.normal_index + 2]);
                        vertexAttributes.push_back(0.0f); // Homogeneous coordinate for normal
                    }
                    else
                    {
                        vertexAttributes.push_back(0.0f);
                        vertexAttributes.push_back(0.0f);
                        vertexAttributes.push_back(0.0f);
                        vertexAttributes.push_back(0.0f);
                    }

                    // Texture coordinates
                    if (idx.texcoord_index >= 0)
                    {
                        vertexAttributes.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                        vertexAttributes.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                    }
                    else
                    {
                        vertexAttributes.push_back(0.0f);
                        vertexAttributes.push_back(0.0f);
                    }

                    indices.push_back(vertexAttributes.size() / 10 - 1);
                }
            }
        }
        return true;
    }
};
