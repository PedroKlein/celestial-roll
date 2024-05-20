#pragma once

#include "mesh.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class MeshManager
{
  public:
    static MeshManager &getInstance()
    {
        static MeshManager instance;
        return instance;
    }

    std::shared_ptr<Mesh> getMesh(const std::string &filename)
    {
        auto it = meshes.find(filename);
        if (it != meshes.end())
        {
            return it->second;
        }
        else
        {
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(filename);
            meshes[filename] = mesh;
            return mesh;
        }
    }

    MeshManager(MeshManager const &) = delete;
    void operator=(MeshManager const &) = delete;

  private:
    MeshManager()
    {
    }

    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
};