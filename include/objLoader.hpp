#pragma once

#include <iostream>
#include <string>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

class ObjLoader
{
  public:
    ObjLoader(const std::string &filename) : filename(filename)
    {
    }

    bool LoadModel()
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

        return LoadAttributes(attrib, shapes);
    }

    const std::vector<float> &GetVertices() const
    {
        return vertices;
    }

    const std::vector<unsigned int> &GetIndices() const
    {
        return indices;
    }

    const std::vector<float> &GetNormals() const
    {
        return normals;
    }

    const std::vector<float> &GetTexCoords() const
    {
        return texcoords;
    }

  private:
    std::string filename;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;
    std::vector<float> texcoords;

    bool LoadAttributes(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes)
    {
        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

                if (!attrib.normals.empty())
                {
                    normals.push_back(attrib.normals[3 * index.normal_index + 0]);
                    normals.push_back(attrib.normals[3 * index.normal_index + 1]);
                    normals.push_back(attrib.normals[3 * index.normal_index + 2]);
                }

                if (!attrib.texcoords.empty())
                {
                    texcoords.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                    texcoords.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
                }

                indices.push_back(indices.size());
            }
        }
        return true;
    }
};
