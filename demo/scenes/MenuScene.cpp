#include "MenuScene.h"

void MenuScene::OnEnter()
{
    // Logs when the menu screen becomes active
    Log::Debug("Entering MenuScene");
}

void MenuScene::OnUpdate(float deltaTime)
{
    // Draws the menu screen and waits for the start input
    UI::Print("PIXELSTORM", Vec2(320.0f, 80.0f), Colors::Blue(), 4.0f, false, TextAnchor::MiddleCenter);
    UI::Print("Press Space To Start", Vec2(320.0f, 170.0f), Colors::White(), 1.5f, false, TextAnchor::MiddleCenter);

    if (Input::IsKeyDown(Key::Space))
    {
        ChangeScene("game");
    }
}

void MenuScene::OnExit()
{
    // Logs when the menu screen is left
    Log::Debug("Leaving MenuScene");
}
