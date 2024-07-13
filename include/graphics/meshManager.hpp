#pragma once

#include <string>
#include <unordered_map>
#include "mesh.hpp"

class MeshManager {
public:
    static MeshManager &getInstance() {
        static MeshManager instance;
        return instance;
    }

    std::shared_ptr<Mesh> getMesh(const std::string &filename) {
        if (const auto it = meshes.find(filename); it != meshes.end()) {
            return it->second;
        }

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(filename);
        meshes[filename] = mesh;
        return mesh;
    }

    MeshManager(MeshManager const &) = delete;

    void operator=(MeshManager const &) = delete;

private:
    MeshManager() {}

    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
};
