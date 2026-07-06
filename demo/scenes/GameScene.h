#pragma once

#include "pixelstorm/PixelStorm.h"

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

    struct Enemy
    {
        Entity EntityHandle; // Enemy entity
        bool Dead = false;   // Enemy removal flag
    };

    void CreatePlayer();                                                                              // Creates the player actor
    void CreateGun();                                                                                 // Creates the gun sprite
    void CreateEnemy();                                                                               // Creates an enemy actor
    Vec2 GetPlayerPosition();                                                                         // Returns the player position
    Vec2 GetGunPosition();                                                                            // Returns the gun position
    Vec2 GetAimDirection(const Vec2 &gunPosition);                                                    // Returns the aim vector
    Vec2 GetBulletDirection(const Vec2 &aimDirection) const;                                          // Returns a safe shooting direction
    Vec2 GetEnemySpawnPosition();                                                                     // Returns a position outside the camera view
    void UpdatePlayerMovement(const Vec2 &movement);                                                  // Applies movement to the player
    void UpdatePlayerCollision();                                                                     // Check for player-enemy collisions
    void UpdateGun(const Vec2 &gunPosition, float aimAngleDegrees);                                   // Updates the gun transform
    void HandleShooting(const Vec2 &gunPosition, const Vec2 &bulletDirection, float aimAngleDegrees); // Spawns a bullet when needed
    void UpdateBullets(float deltaTime);                                                              // Updates active bullets
    void UpdateEnemies(float deltaTime);                                                              // Updates active enemies
    void UpdateEnemySpawner(float deltaTime);                                                         // Spawns enemies over time
    void UpdatePlayerAnimation(const Vec2 &movement);                                                 // Updates player animation
    void UpdateHud();                                                                                 // Draws the alive-time label
    float GetEnemySpawnDelay() const;                                                                 // Returns the current enemy spawn delay

    Entity player;                  // Player actor
    Entity gun;                     // Gun sprite
    Array<Bullet> bullets;          // Active bullets
    Array<Enemy> enemies;           // Active enemies
    double playerSpawnTime = 0.0;   // Player spawn time
    float m_EnemySpawnTimer = 0.0f; // Enemy spawn timer
};
