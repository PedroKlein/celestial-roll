#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <tiny_obj_loader.h>

struct VertexAttribute {
    glm::vec4 position;
    glm::vec4 normal;
    glm::vec2 texcoord;
};

class ObjLoader {
public:
    explicit ObjLoader(std::string filename) : filename(std::move(filename)) {}

    bool loadModel() {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str())) {
            std::cerr << "TinyObjLoader Error: " << err << std::endl;
            return false;
        }

        if (!err.empty()) {
            std::cerr << "TinyObjLoader Warning: " << err << std::endl;
        }

        return loadAttributes(attrib, shapes);
    }

    [[nodiscard]] const std::vector<VertexAttribute> &getVertexAttributes() const { return vertexAttributes; }
    [[nodiscard]] const std::vector<unsigned int> &getIndices() const { return indices; }

private:
    std::string filename;
    std::vector<VertexAttribute> vertexAttributes;
    std::vector<unsigned int> indices;

    bool loadAttributes(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes) {
        for (const auto &[name, mesh]: shapes) {
            for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
                const int fv = mesh.num_face_vertices[f];

                for (int v = 0; v < fv; v++) {
                    const tinyobj::index_t idx = mesh.indices[f * fv + v];
                    VertexAttribute va{};

                    va.position = glm::vec4(attrib.vertices[3 * idx.vertex_index + 0],
                                            attrib.vertices[3 * idx.vertex_index + 1],
                                            attrib.vertices[3 * idx.vertex_index + 2], 1.0f);

                    va.normal = idx.normal_index >= 0 ? glm::vec4(attrib.normals[3 * idx.normal_index + 0],
                                                                  attrib.normals[3 * idx.normal_index + 1],
                                                                  attrib.normals[3 * idx.normal_index + 2], 0.0f)
                                                      : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

                    va.texcoord = idx.texcoord_index >= 0 ? glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0],
                                                                      attrib.texcoords[2 * idx.texcoord_index + 1])
                                                          : glm::vec2(0.0f, 0.0f);

                    vertexAttributes.push_back(va);
                    indices.push_back(vertexAttributes.size() - 1);
                }
            }
        }
        return true;
    }
};
