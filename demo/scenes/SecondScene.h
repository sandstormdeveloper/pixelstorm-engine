#pragma once

#include "pixelstorm/PixelStorm.h"

class SecondScene : public Scene
{
public:
    void OnEnter(SceneContext& context) override;
    void OnUpdate(SceneContext& context, float deltaTime) override;

private:
    Entity m_Box;
};