#pragma once

// forward declarations
class Transform;
class Collider;
class PhysicsMaterial;
class GravityComponent;
class RigidBody;
class Renderer;

// order of update is important
enum class ComponentType
{
    Transform,
    Collider,
    PhysicsMaterial,
    Gravity,
    RigidBody,
    Renderer,
};

template <typename T> ComponentType getComponentType();