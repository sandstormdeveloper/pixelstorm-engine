#pragma once

#include "pixelstorm/PixelStorm.h"

#include <vector>

class GameScene : public Scene
{
public:
    void OnEnter() override;
    void OnUpdate(float deltaTime) override;
    void OnExit() override;

private:
    struct Bullet
    {
        Entity EntityHandle; // Bullet entity
        Vec2 Velocity;       // Bullet velocity
        bool Dead = false;   // Bullet removal flag
    };

    void CreatePlayer();                                                                              // Creates the player actor
    void CreateGun();                                                                                 // Creates the gun sprite
    Vec2 GetPlayerPosition();                                                                         // Returns the player position
    Vec2 GetGunPosition();                                                                            // Returns the gun position
    Vec2 GetAimDirection(const Vec2 &gunPosition);                                                    // Returns the aim vector
    Vec2 GetBulletDirection(const Vec2 &aimDirection) const;                                          // Returns a safe shooting direction
    void UpdatePlayerMovement(const Vec2 &movement);                                                  // Applies movement to the player
    void UpdateGun(const Vec2 &gunPosition, float aimAngleDegrees);                                   // Updates the gun transform
    void HandleShooting(const Vec2 &gunPosition, const Vec2 &bulletDirection, float aimAngleDegrees); // Spawns a bullet when needed
    void UpdateBullets(float deltaTime);                                                              // Updates active bullets
    void UpdatePlayerAnimation(const Vec2 &movement);                                                 // Updates player animation
    void UpdateHud();                                                                                 // Draws the alive-time label

    Entity player;                // Player actor
    Entity gun;                   // Gun sprite
    Array<Bullet> bullets;        // Active bullets
    double playerSpawnTime = 0.0; // Player spawn time
};
