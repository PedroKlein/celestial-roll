#include "game/component.hpp"
#include "game/gameObject.hpp"

void Component::setGameObject(GameObject *obj)
{
    gameObject = obj;
}

void Component::enable()
{
    enabled = true;
}
void Component::disable()
{
    enabled = false;
}

bool Component::isEnabled() const
{
    return enabled;
}

const GameObject *Component::getGameObject() const
{
    return gameObject;
}