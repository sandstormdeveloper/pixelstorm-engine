#pragma once

#include "pixelstorm/PixelStorm.h"

class GameScene : public Scene
{
public:
    void OnEnter() override;
    void OnUpdate(float deltaTime) override;
    void OnExit() override;

private:
    Entity m_Player;        // Main controllable actor
    Entity m_PlayerEmitter; // Helper emitter used for player feedback
    Entity m_DustEmitter;   // Ambient emitter for the scene
    Entity m_Wall;          // Static obstacle used for collision tests
    Entity m_Crate;         // Dynamic object used for physics tests
    Entity m_TriggerZone;   // Trigger volume used to exercise callbacks
};
