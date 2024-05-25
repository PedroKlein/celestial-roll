#include "graphics/renderer.hpp"
#include "game/gameObject.hpp"

Renderer::Renderer(const std::shared_ptr<Mesh> &mesh) : mesh(mesh)
{
}

void Renderer::initialize()
{
    auto transform = gameObject->getComponent<Transform>();
    if (!transform)
    {
        std::cerr << "Renderer requires a Transform component." << std::endl;
    }

    interpolatedTransform = std::make_shared<InterpolatedTransform>(transform.get());
}

void Renderer::update(float alpha)
{
    if (!mesh || !interpolatedTransform)
    {
        std::cerr << "Renderer component is not properly initialized." << std::endl;
        return;
    }

    interpolatedTransform->calculateInterpolation(alpha);

    _globalShader.setMat4("model", interpolatedTransform->getInterpolatedModelMatrix());
    mesh->draw(_globalShader);
}