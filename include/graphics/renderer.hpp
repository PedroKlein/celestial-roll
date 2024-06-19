#pragma once

#include "game/component.hpp"
#include "graphics/material.hpp"
#include "graphics/mesh.hpp"
#include "interpolatedTransform.hpp"
#include "matrixUtils.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

class Renderer : public Component
{
  public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    std::shared_ptr<InterpolatedTransform> interpolatedTransform;

    Renderer(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material);

    void initialize() override;

    void update(float alpha) override;
};