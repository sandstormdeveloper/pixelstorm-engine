#pragma once

#include "pixelstorm/ecs/Entity.h"

class Registry
{
public:
    Registry();

    Entity CreateEntity(); // Creates a new entity with a unique ID

private:
    EntityId m_NextEntityId; // Next entity ID
};