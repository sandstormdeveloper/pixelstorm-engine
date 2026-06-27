#pragma once

#include "pixelstorm/PixelStorm.h"

class SecondScene : public Scene
{
public:
    void OnEnter() override;
    void OnUpdate(float deltaTime) override;
    void OnExit() override;

private:
    Entity m_Player;        // Actor that moves through the trigger volume
    Entity m_PlayerEmitter; // Helper emitter used for trigger feedback
    Entity m_TriggerZone;   // Trigger volume used for the scene logic
    bool m_IsInsideTrigger = false;
};
