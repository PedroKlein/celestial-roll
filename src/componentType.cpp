#include "game/componentType.hpp"
#include "collision/boxCollider.hpp"
#include "collision/collider.hpp"
#include "collision/sphereCollider.hpp"
#include "graphics/renderer.hpp"
#include "physics/PhysicsMaterial.hpp"
#include "physics/gravityComponent.hpp"
#include "physics/rigidBody.hpp"
#include "transform.hpp"

template <> ComponentType getComponentType<Transform>()
{
    return ComponentType::Transform;
}

template <> ComponentType getComponentType<Collider>()
{
    return ComponentType::Collider;
}

template <> ComponentType getComponentType<BoxCollider>()
{
    return ComponentType::Collider;
}

template <> ComponentType getComponentType<SphereCollider>()
{
    return ComponentType::Collider;
}

template <> ComponentType getComponentType<PhysicsMaterial>()
{
    return ComponentType::PhysicsMaterial;
}

template <> ComponentType getComponentType<GravityComponent>()
{
    return ComponentType::Gravity;
}

template <> ComponentType getComponentType<RigidBody>()
{
    return ComponentType::RigidBody;
}

template <> ComponentType getComponentType<Renderer>()
{
    return ComponentType::Renderer;
}