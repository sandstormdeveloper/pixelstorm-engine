#include "GameOverScene.h"

void GameOverScene::OnEnter()
{
    // Logs when the game over screen becomes active
    Log::Debug("Entering GameOverScene");
}

void GameOverScene::OnUpdate(float deltaTime)
{
    // Draws the game over screen and waits for a retry input
    UI::Print("GAME OVER", Vec2(320.0f, 80.0f), Colors::Blue(), 4.0f, false, TextAnchor::MiddleCenter);
    UI::Print("Press Space To Retry", Vec2(320.0f, 170.0f), Colors::White(), 1.5f, false, TextAnchor::MiddleCenter);

    if (Input::IsKeyDown(Key::Space))
    {
        ChangeScene("game");
    }
}

void GameOverScene::OnExit()
{
    // Logs when the game over screen is left
    Log::Debug("Leaving GameOverScene");
}
