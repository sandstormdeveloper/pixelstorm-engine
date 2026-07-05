#include "SecondScene.h"

void SecondScene::OnEnter()
{
    Log::Debug("Entering SecondScene");

    // Creates a lightweight actor that moves on its own to test trigger events
    m_Player = GetWorld().CreateAnimatedActor(
        "Trigger Tester",
        Vec2(120.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White(),
        "player_run",
        {
            { "walk", { glm::ivec2(32, 32), 4, 4, 8.0f, true, 0 } }
        },
        "walk"
    );

    m_Player.Rigidbody().SetVelocity(Vec2(90.0f, 0.0f));
    m_Player.Sprite().SetRenderOrder(100);

    // Keeps the main camera centered on the moving actor while this scene is active
    GetApplication().FollowCamera(m_Player);

    // Gives the moving actor a lightweight burst emitter for trigger feedback
    m_PlayerEmitter = GetWorld().CreateParticleEmitter(
        "TriggerTesterParticles",
        m_Player.Transform().GetPosition(),
        "wall",
        8,
        0.35f,
        90.0f,
        30.0f,
        180.0f,
        glm::vec4(0.55f, 0.95f, 1.0f, 1.0f),
        glm::vec4(0.10f, 0.50f, 1.0f, 0.0f),
        Vec2(5.0f, 5.0f),
        Vec2(1.5f, 1.5f),
        0.0f,
        50,
        false,
        false,
        0.0f
    );
    // Creates a trigger volume that the moving actor will cross repeatedly
    m_TriggerZone = GetWorld().CreateStaticBox(
        "Second Scene Trigger",
        Vec2(320.0f, 180.0f),
        Vec2(96.0f, 96.0f),
        Colors::Green(),
        "wall",
        true
    );

    // Registers trigger callbacks on the trigger zone itself
    m_TriggerZone.Trigger().SetOnEnter([this](Entity other) {
        if (other.GetId() == m_Player.GetId())
        {
            Log::Debug("Player entered the trigger zone.");
            m_PlayerEmitter.Transform().SetPosition(other.Transform().GetPosition());
            m_PlayerEmitter.Particles().EmitBurst(8);
            m_Player.Sprite().SetColor(Colors::Green());
        }   
    });

    m_TriggerZone.Trigger().SetOnExit([this](Entity other) {
        if (other.GetId() == m_Player.GetId())
        {
            Log::Debug("Player exited the trigger zone.");
            m_Player.Sprite().SetColor(Colors::White());
        }
    });
}

void SecondScene::OnUpdate(float deltaTime)
{
    (void)deltaTime;

    // Keeps the particle helper aligned with the moving actor
    m_PlayerEmitter.Transform().SetPosition(m_Player.Transform().GetPosition());

    // Reverses direction when the actor reaches either side of the screen
    if (m_Player.Transform().GetPosition().x < 80.0f)
    {
        m_Player.Rigidbody().SetVelocity(Vec2(90.0f, 0.0f));
    }
    else if (m_Player.Transform().GetPosition().x > 560.0f)
    {
        m_Player.Rigidbody().SetVelocity(Vec2(-90.0f, 0.0f));
    }

    if (Input::IsActionJustPressed("cancel") || Input::IsActionJustPressed("interact"))
    {
        ChangeScene("game");
    }
}

void SecondScene::OnExit()
{
    Log::Debug("Leaving SecondScene");
    GetApplication().StopCameraFollow();
}
