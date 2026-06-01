#include "SecondScene.h"

void SecondScene::OnEnter()
{
    Log::Debug("Entering SecondScene");
    
    m_Box = GetWorld().CreateStaticBox(
        "Second Scene Box",
        Vec2(320.0f, 180.0f),
        Vec2(64.0f, 64.0f),
        Colors::Green()
    );

    m_Box.Sprite().SetTexture("wall");
}

void SecondScene::OnUpdate(float deltaTime)
{
    if (Input::IsActionJustPressed("cancel") || Input::IsActionJustPressed("interact"))
    {
        ChangeScene("game");
    }
}

void SecondScene::OnExit()
{
    Log::Debug("Leaving SecondScene");
}
