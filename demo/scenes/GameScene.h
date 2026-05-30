#pragma once

#include "pixelstorm/PixelStorm.h"

class GameScene : public Scene
{
public:
    void OnEnter(SceneContext& context) override;
    void OnUpdate(SceneContext& context, float deltaTime) override;
    void OnExit(SceneContext& context) override;

private:
    Entity m_Player;
    Entity m_Wall;
};