#include "GameScene.h"

void GameScene::OnEnter()
{
    Log::Debug("Entering GameScene");

    // Creates the main player actor used to test movement and collisions
    m_Player = GetWorld().CreateActor(
        "Player",
        Vec2(160.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White()
    );

    m_Player.Sprite().SetTexture("player");

    // Creates a visible wall that can stop the player
    m_Wall = GetWorld().CreateStaticBox(
        "Wall",
        Vec2(240.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White()
    );

    m_Wall.Sprite().SetTexture("wall");

    // Creates a dynamic crate so dynamic-vs-dynamic resolution is also exercised
    m_Crate = GetWorld().CreateActor(
        "Crate",
        Vec2(320.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White()
    );

    m_Crate.Sprite().SetTexture("wall");

    // Creates a trigger area that only reports overlaps
    m_TriggerZone = GetWorld().CreateStaticBox(
        "Game Trigger",
        Vec2(460.0f, 180.0f),
        Vec2(48.0f, 96.0f),
        Colors::Blue(),
        true
    );

    m_TriggerZone.Sprite().SetTexture("wall");
    m_TriggerZone.Sprite().SetColor(Colors::Blue());

    // Registers trigger callbacks directly on the trigger zone
    m_TriggerZone.Trigger().SetOnEnter([this](Entity other) {
        if (other.GetId() == m_Player.GetId())
        {
            Log::Debug("Player entered the GameScene trigger zone.");
            m_Player.Sprite().SetColor(Colors::Green());
        }
    });

    m_TriggerZone.Trigger().SetOnExit([this](Entity other) {
        if (other.GetId() != m_Player.GetId())
        {
            Log::Debug("Player exited the GameScene trigger zone.");
            m_Player.Sprite().SetColor(Colors::White());
        }
    });
}

void GameScene::OnUpdate(float deltaTime)
{
    // Reads the player input axis and converts it into horizontal or vertical movement
    const float speed = 120.0f;
    const Vec2 movement = Input::GetAxis2D("move");

    m_Player.Rigidbody().SetVelocity(movement * speed);

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
        m_Player.Rigidbody().SetVelocity(Vec2(0.0f));
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
        ChangeScene("second");
    }
}

void GameScene::OnExit()
{
    Log::Debug("Leaving GameScene");
}
