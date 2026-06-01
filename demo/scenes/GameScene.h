#pragma once

#include "pixelstorm/PixelStorm.h"

class GameScene : public Scene
{
public:
    void OnEnter() override;
    void OnUpdate(float deltaTime) override;
    void OnExit() override;

private:
    Entity m_Player;
    Entity m_Wall;
};
