#include "graphics/renderer.hpp"
#include "game/gameObject.hpp"

Renderer::Renderer(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material)
    : mesh(mesh), material(material)
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

    material->shader->use();

    material->shader->setMat4("model", interpolatedTransform->getInterpolatedModelMatrix());
    mesh->draw(*material.get());
}