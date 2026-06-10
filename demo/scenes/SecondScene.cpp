#include "SecondScene.h"

void SecondScene::OnEnter()
{
    Log::Debug("Entering SecondScene");

    // Creates a lightweight actor that moves on its own to test trigger events
    m_Player = GetWorld().CreateActor(
        "Trigger Tester",
        Vec2(120.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White(),
        "player"
    );

    m_Player.Rigidbody().SetVelocity(Vec2(90.0f, 0.0f));
    m_Player.Sprite().SetRenderOrder(100);

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
}
