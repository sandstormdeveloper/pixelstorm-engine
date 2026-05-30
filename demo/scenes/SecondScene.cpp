#include "SecondScene.h"

void SecondScene::OnEnter(SceneContext& context)
{
    World& world = *context.GameWorld;

    m_Box = world.CreateStaticBox(
        "Second Scene Box",
        Vec2(320.0f, 180.0f),
        Vec2(64.0f, 64.0f),
        Colors::Green()
    );

    m_Box.Sprite().SetTexture("wall");
}

void SecondScene::OnUpdate(SceneContext& context, float deltaTime)
{
    if (Input::IsActionJustPressed("cancel") || Input::IsActionJustPressed("interact"))
    {
        context.Scenes->ChangeScene("game");
    }
}