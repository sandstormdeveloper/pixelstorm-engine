#include "GameScene.h"

void GameScene::OnEnter(SceneContext& context)
{
    World& world = *context.GameWorld;

    m_Player = world.CreateActor(
        "Player",
        Vec2(160.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White()
    );

    m_Player.Sprite().SetTexture("player");

    m_Wall = world.CreateStaticBox(
        "Wall",
        Vec2(240.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White()
    );

    m_Wall.Sprite().SetTexture("wall");
}

void GameScene::OnUpdate(SceneContext& context, float deltaTime)
{
    const float speed = 120.0f;
    const Vec2 movement = Input::GetAxis2D("move");

    m_Player.Transform().Translate(movement * speed * deltaTime);

    if (Input::IsActionJustPressed("jump"))
    {
        m_Player.Sprite().SetColor(Colors::Green());
    }

    if (Input::IsActionJustReleased("jump"))
    {
        m_Player.Sprite().SetColor(Colors::White());
    }

    if (Input::IsMouseButtonJustPressed(MouseButton::Left))
    {
        m_Player.Transform().SetPosition(Input::GetMousePosition());
    }

    if (Input::IsMouseButtonDown(MouseButton::Right))
    {
        m_Player.Sprite().SetColor(Colors::Blue());
    }

    if (Input::IsMouseButtonJustReleased(MouseButton::Right))
    {
        m_Player.Sprite().SetColor(Colors::White());
    }

    if (Input::IsActionJustPressed("interact"))
    {
        context.Scenes->ChangeScene("second");
    }
}

void GameScene::OnExit(SceneContext& context)
{
}
