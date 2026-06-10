#pragma once

#include <functional>
#include <glm/vec2.hpp>

class Entity;

struct Collider
{
    glm::vec2 Size;                             // Collider size
    glm::vec2 Offset;                           // Offset relative to transform
    bool IsTrigger;                             // Detects collisions, but doesn't block movement
    std::function<void(Entity)> OnTriggerEnter; // Called when a trigger starts overlapping
    std::function<void(Entity)> OnTriggerStay;  // Called while a trigger keeps overlapping
    std::function<void(Entity)> OnTriggerExit;  // Called when a trigger stops overlapping

    Collider();
    Collider(const glm::vec2 &size, const glm::vec2 &offset = glm::vec2(0.0f), bool isTrigger = false);
};
