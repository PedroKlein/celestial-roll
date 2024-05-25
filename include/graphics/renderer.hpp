#pragma once
#include "globals.hpp"

#include "game/component.hpp"
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
    std::shared_ptr<InterpolatedTransform> interpolatedTransform;

    Renderer(const std::shared_ptr<Mesh> &mesh);

    void initialize() override;

    void update(float alpha) override;
};