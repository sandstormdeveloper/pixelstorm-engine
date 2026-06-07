#pragma once

#include "pixelstorm/core/Math.h"

class Registry;
struct Rigidbody;

class PhysicsSystem
{
public:
    PhysicsSystem();

    void Update(Registry &registry, float deltaTime); // Updates rigidbodies and resolves simple collisions

    void SetGravity(const Vec2 &gravity); // Sets world gravity
    Vec2 GetGravity() const;              // Returns world gravity

private:
    void ApplyGravity(Rigidbody &rigidbody, float deltaTime); // Applies gravity to one body

    Vec2 m_Gravity; // Gravity acceleration used by platform-style bodies
};
