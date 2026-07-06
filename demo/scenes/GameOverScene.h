#pragma once

#include "pixelstorm/PixelStorm.h"

class GameOverScene : public Scene
{
public:
    void OnEnter() override;                 // Initializes the game over screen
    void OnUpdate(float deltaTime) override; // Handles retry input and HUD drawing
    void OnExit() override;                  // Cleans up the scene
};
