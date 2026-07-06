#include "GameScene.h"

#include "pixelstorm/core/Time.h"
#include "pixelstorm/renderer/Camera2D.h"

void GameScene::CreatePlayer()
{
    // Builds the main controllable actor for the scene
    player = GetWorld().CreateAnimatedActor(
        "Player",
        Vec2(160.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White(),
        "player",
        {
            { "idle", { iVec2(8, 8), 1, 4, 0.0f, true, 0 } },
            { "walk", { iVec2(8, 8), 4, 4, 10.0f, true, 4 } }
        },
        "idle"
    );

    player.Collider().SetSize(Vec2(16.0f, 28.0f));
    player.Collider().SetOffset(Vec2(0.0f, 2.0f));
    player.Sprite().SetRenderOrder(5);
}

void GameScene::CreateGun()
{
    // Builds the sprite that is visually attached to the player
    gun = GetWorld().CreateSprite(
        "Gun",
        player.Transform().GetPosition(),
        Vec2(12.0f, 8.0f),
        Colors::White(),
        "gun"
    );

    gun.Sprite().SetRenderOrder(10);
    gun.Transform().SetPivot(Vec2(-0.5f, 0.0f));
}

Vec2 GameScene::GetPlayerPosition()
{
    // Returns the current player position for shared scene logic
    return player.Transform().GetPosition();
}

Vec2 GameScene::GetGunPosition()
{
    // Keeps the weapon slightly above the player sprite
    return GetPlayerPosition() + Vec2(0.0f, 8.0f);
}

Vec2 GameScene::GetAimDirection(const Vec2 &gunPosition)
{
    // Returns the vector from the gun to the current mouse position
    return Input::GetMouseWorldPosition(GetApplication().GetCamera()) - gunPosition;
}

Vec2 GameScene::GetBulletDirection(const Vec2 &aimDirection) const
{
    // Returns a safe normalized shooting direction
    Vec2 bulletDirection = Normalize(aimDirection);
    if (bulletDirection.x == 0.0f && bulletDirection.y == 0.0f)
    {
        return Vec2(1.0f, 0.0f);
    }

    return bulletDirection;
}

void GameScene::UpdatePlayerMovement(const Vec2 &movement)
{
    // Applies player movement directly through the rigidbody
    const float speed = 120.0f;
    player.Rigidbody().SetVelocity(movement * speed);
}

void GameScene::UpdateGun(const Vec2 &gunPosition, float aimAngleDegrees)
{
    // Mirrors the weapon position and rotation to match the aim direction
    if (aimAngleDegrees < -100 || aimAngleDegrees > 100)
    {
        gun.Sprite().FlipY(true);
    }
    else if (aimAngleDegrees > -80 || aimAngleDegrees < 80)
    {
        gun.Sprite().FlipY(false);
    }

    gun.Transform().SetPosition(gunPosition);
    gun.Transform().SetRotation(aimAngleDegrees);
}

void GameScene::HandleShooting(const Vec2 &gunPosition, const Vec2 &bulletDirection, float aimAngleDegrees)
{
    // Spawns a bullet when the left mouse button is pressed
    if (!Input::IsMouseButtonJustPressed(MouseButton::Left))
    {
        return;
    }

    Entity bullet = GetWorld().CreateActor(
        "Bullet",
        gunPosition + bulletDirection * 16.0f,
        Vec2(4.0f, 4.0f),
        Colors::White(),
        "bullet"
    );

    bullet.Collider().SetTrigger(true);
    bullet.Sprite().SetRenderOrder(20);
    bullet.Transform().SetRotation(aimAngleDegrees);
    Add(bullets, Bullet{ bullet, bulletDirection * 360.0f, false });
}

void GameScene::UpdateBullets(float deltaTime)
{
    // Advances every active bullet and marks the ones that should be removed
    ForEach(bullets, [&](Bullet &bullet) {
        if (bullet.Dead || !bullet.EntityHandle.IsValid())
        {
            bullet.Dead = true;
            return;
        }

        bullet.EntityHandle.Transform().Translate(bullet.Velocity * deltaTime);

        bullet.EntityHandle.Trigger().SetOnEnter([&](Entity other)
        {
            if (other.GetName() == "Enemy")
            {
                other.Destroy();
            }
        });

        const Vec2 bulletPosition = bullet.EntityHandle.Transform().GetPosition();
        if (GetApplication().IsPositionOutsideCamera(bulletPosition, 32.0f))
        {
            // Destroys bullets once they leave the visible play area
            bullet.EntityHandle.Destroy(false);
            bullet.Dead = true;
        }
    });

    RemoveIf(bullets, [](const Bullet &bullet) {
        return bullet.Dead;
    });
}

void GameScene::UpdatePlayerAnimation(const Vec2 &movement)
{
    // Chooses the player animation and facing based on movement input
    if (movement.x != 0.0f || movement.y != 0.0f)
    {
        player.Animation().Play("walk");

        if (movement.x > 0.1f)
        {
            player.Sprite().FlipX(false);
        }

        if (movement.x < 0.1f)
        {
            player.Sprite().FlipX(true);
        }
    }
    else
    {
        player.Animation().Play("idle");
    }
}

void GameScene::UpdateHud()
{
    // Displays how long the player has been alive
    UI::Print("Alive: " + ToString(Time::GetElapsedTime() - playerSpawnTime) + "s", Vec2(10.0f, -10.0f), Colors::White(), 1.5f, false);
}

void GameScene::OnEnter()
{
    Log::Debug("Entering GameScene");
    playerSpawnTime = Time::GetElapsedTime();

    CreatePlayer();
    GetApplication().FollowCamera(player);
    CreateGun();
}

void GameScene::OnUpdate(float deltaTime)
{
    if (!player.IsValid())
    {
        return;
    }

    const Vec2 movement = Input::GetAxis2D("move");
    UpdatePlayerMovement(movement);

    const Vec2 gunPosition = GetGunPosition();
    const Vec2 aimDirection = GetAimDirection(gunPosition);
    const float aimAngleDegrees = Degrees(Atan2(aimDirection.y, aimDirection.x));
    const Vec2 bulletDirection = GetBulletDirection(aimDirection);

    HandleShooting(gunPosition, bulletDirection, aimAngleDegrees);
    UpdateGun(gunPosition, aimAngleDegrees);
    UpdateHud();
    UpdateBullets(deltaTime);
    UpdatePlayerAnimation(movement);
}

void GameScene::OnExit()
{
    bullets.clear();
    Log::Debug("Leaving GameScene");
}
