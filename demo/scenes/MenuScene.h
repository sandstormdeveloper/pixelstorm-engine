#pragma once

#include "pixelstorm/PixelStorm.h"

class MenuScene : public Scene
{
public:
    void OnEnter() override;                 // Initializes the menu screen
    void OnUpdate(float deltaTime) override; // Handles start input and HUD drawing
    void OnExit() override;                  // Cleans up the scene
};
