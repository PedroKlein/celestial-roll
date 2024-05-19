#pragma once

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

        return loadAttributes(attrib, shapes);
    }

    const std::vector<float> &getVertices() const
    {
        return vertices;
    }
    const std::vector<unsigned int> &getIndices() const
    {
        return indices;
    }
    const std::vector<float> &getNormals() const
    {
        return normals;
    }
    const std::vector<float> &getTexCoords() const
    {
        return texcoords;
    }

  private:
    std::string filename;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;
    std::vector<float> texcoords;

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

                    vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                    vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                    vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                    vertices.push_back(1.0f); // Homogeneous coordinate for vertex

                    indices.push_back(vertices.size() / 4 - 1);
                    if (idx.normal_index >= 0)
                    {
                        normals.push_back(attrib.normals[3 * idx.normal_index + 0]);
                        normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
                        normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
                    }

                    if (idx.texcoord_index >= 0)
                    {
                        texcoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                        texcoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                    }
                }
            }
        }

        return true;
    }
};
