#include "GameScene.h"

void GameScene::OnEnter()
{
    Log::Debug("Entering GameScene");

    // Creates the main player actor used to test movement and collisions
    m_Player = GetWorld().CreateAnimatedActor(
        "Player",
        Vec2(160.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White(),
        "player_run",
        {
            { "idle", { glm::ivec2(32, 32), 4, 4, 0.0f, true, 0 } },
            { "walk", { glm::ivec2(32, 32), 4, 4, 10.0f, true, 0 } }
        },
        "idle"
    );

    m_Player.Sprite().SetRenderOrder(100);

    // Keeps the main camera centered on the player while this scene is active
    GetApplication().FollowCamera(m_Player);

    // Gives the player a small burst emitter used for jump and trigger feedback
    m_PlayerEmitter = GetWorld().CreateParticleEmitter(
        "PlayerParticles",
        Vec2(160.0f, 180.0f),
        "wall",
        10,
        0.45f,
        110.0f,
        40.0f,
        220.0f,
        glm::vec4(1.0f, 0.85f, 0.25f, 1.0f),
        glm::vec4(1.0f, 0.20f, 0.05f, 0.0f),
        Vec2(6.0f, 6.0f),
        Vec2(2.0f, 2.0f),
        0.0f,
        50,
        false,
        false,
        0.0f
    );
    // Creates a standalone ambient emitter so the demo shows the world-level helper in action
    m_DustEmitter = GetWorld().CreateParticleEmitter(
        "Dust",
        Vec2(200.0f, 330.0f),
        "wall",
        3,
        0.9f,
        18.0f,
        10.0f,
        360.0f,
        glm::vec4(0.85f, 0.85f, 0.90f, 0.60f),
        glm::vec4(0.85f, 0.85f, 0.90f, 0.0f),
        Vec2(4.0f, 4.0f),
        Vec2(1.0f, 1.0f),
        0.0f,
        10,
        true,
        true,
        1.5f
    );

    // Creates a visible wall that can stop the player
    m_Wall = GetWorld().CreateStaticBox(
        "Wall",
        Vec2(240.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White(),
        "wall"
    );

    // Creates a dynamic crate so dynamic-vs-dynamic resolution is also exercised
    m_Crate = GetWorld().CreateActor(
        "Crate",
        Vec2(320.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White(),
        "wall"
    );

    // Creates a trigger area that only reports overlaps
    m_TriggerZone = GetWorld().CreateStaticBox(
        "Game Trigger",
        Vec2(460.0f, 180.0f),
        Vec2(48.0f, 96.0f),
        Colors::Blue(),
        "wall",
        true
    );

    // Registers trigger callbacks directly on the trigger zone
    m_TriggerZone.Trigger().SetOnEnter([this](Entity other) {
        if (other.GetId() == m_Player.GetId())
        {
            Log::Debug("Player entered the trigger zone.");
            m_PlayerEmitter.Transform().SetPosition(other.Transform().GetPosition());
            m_PlayerEmitter.Particles().EmitBurst(12);
            other.Sprite().SetColor(Colors::Green());
            other.Destroy();
            m_Player = Entity();
        }
    });

    m_TriggerZone.Trigger().SetOnExit([this](Entity other) {
        if (other.GetId() == m_Player.GetId())
        {
            Log::Debug("Player exited the trigger zone.");
            other.Sprite().SetColor(Colors::White());
        }
    });
}

void GameScene::OnUpdate(float deltaTime)
{
    if (!m_Player.IsValid())
    {
        return;
    }

    // Shows a small overlay using the shared UI helper
    UI::Print("Press Interact to change scene", Vec2(16.0f, 16.0f), Colors::White(), 1.0f, false);

    // Keeps the particle helper attached to the player while the actor exists
    m_PlayerEmitter.Transform().SetPosition(m_Player.Transform().GetPosition());

    // Reads the player input axis and converts it into horizontal or vertical movement
    const float speed = 120.0f;
    const Vec2 movement = Input::GetAxis2D("move");

    m_Player.Rigidbody().SetVelocity(movement * speed);

    if (movement.x != 0.0f || movement.y != 0.0f)
    {
        m_Player.Animation().Play("walk");
    }
    else
    {
        m_Player.Animation().Play("idle");
    }

    if (Input::IsActionJustPressed("jump"))
    {
        m_Player.Sprite().SetColor(Colors::Green());
        m_PlayerEmitter.Particles().EmitBurst(10);
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
