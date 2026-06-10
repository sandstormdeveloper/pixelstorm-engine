#pragma once

#include "pixelstorm/PixelStorm.h"

class SecondScene : public Scene
{
public:
    void OnEnter() override;
    void OnUpdate(float deltaTime) override;
    void OnExit() override;

private:
    Entity m_Player;
    Entity m_TriggerZone;
    bool m_IsInsideTrigger = false;
};
