#include "GameScene.h"

Vec2 GameScene::GetEnemySpawnPosition()
{
    // Spawns enemies outside the current visible camera area
    const Vec2 cameraPosition = GetApplication().GetCamera().GetPosition();
    const float halfWidth = Window::GetLogicalWidth() * 0.5f;
    const float halfHeight = Window::GetLogicalHeight() * 0.5f;
    const float spawnMargin = 48.0f;

    const int edge = RandomInt(0, 3);
    switch (edge)
    {
    case 0:
        return Vec2(cameraPosition.x - halfWidth - spawnMargin, RandomFloat(cameraPosition.y - halfHeight - spawnMargin, cameraPosition.y + halfHeight + spawnMargin));
    case 1:
        return Vec2(cameraPosition.x + halfWidth + spawnMargin, RandomFloat(cameraPosition.y - halfHeight - spawnMargin, cameraPosition.y + halfHeight + spawnMargin));
    case 2:
        return Vec2(RandomFloat(cameraPosition.x - halfWidth - spawnMargin, cameraPosition.x + halfWidth + spawnMargin), cameraPosition.y - halfHeight - spawnMargin);
    default:
        return Vec2(RandomFloat(cameraPosition.x - halfWidth - spawnMargin, cameraPosition.x + halfWidth + spawnMargin), cameraPosition.y + halfHeight + spawnMargin);
    }
}

float GameScene::GetEnemySpawnDelay() const
{
    // Reduces the spawn delay as the player survives longer
    const float elapsed = static_cast<float>(Time::GetElapsedTime() - playerSpawnTime);
    float delay = 2.5f - (elapsed * 0.03f);

    if (delay < 0.45f)
    {
        delay = 0.45f;
    }

    return delay;
}

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

    player.Collider().SetTrigger(true);
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

void GameScene::CreateEnemy()
{
    // Builds an enemy outside the visible camera area
    Entity enemy = GetWorld().CreateAnimatedActor(
        "Enemy",
        GetEnemySpawnPosition(),
        Vec2(32.0f, 32.0f),
        Colors::White(),
        "enemy",
        {
            { "idle", { iVec2(8, 8), 1, 4, 0.0f, true, 0 } },
            { "walk", { iVec2(8, 8), 4, 4, 10.0f, true, 4 } }
        },
        "walk"
    );

    enemy.Collider().SetSize(Vec2(16.0f, 28.0f));
    enemy.Collider().SetOffset(Vec2(0.0f, 2.0f));
    enemy.Sprite().SetRenderOrder(4);
    Add(enemies, Enemy{ enemy, false });
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

void GameScene::UpdatePlayerCollision()
{
    player.Trigger().SetOnEnter([&](Entity other) {
        if (other.GetName() == "Enemy")
        {
            player.Destroy();
            gun.Destroy();
        }
    });
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
    bullet.Trigger().SetOnEnter([bullet](Entity other) {
        if (other.GetName() == "Enemy")
        {
            other.Destroy();
            bullet.Destroy();
        }
    });

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

void GameScene::UpdateEnemies(float deltaTime)
{
    // Moves enemies toward the player and removes invalid ones
    ForEach(enemies, [&](Enemy &enemy) {
        if (enemy.Dead || !enemy.EntityHandle.IsValid())
        {
            enemy.Dead = true;
            return;
        }

        const Vec2 enemyPosition = enemy.EntityHandle.Transform().GetPosition();
        const Vec2 direction = Normalize(GetPlayerPosition() - enemyPosition);
        const float enemySpeed = 60.0f;

        enemy.EntityHandle.Rigidbody().SetVelocity(direction * enemySpeed);
        enemy.EntityHandle.Animation().Play("walk");
        enemy.EntityHandle.Sprite().FlipX(direction.x < 0.0f);
    });

    RemoveIf(enemies, [](const Enemy &enemy) {
        return enemy.Dead;
    });
}

void GameScene::UpdateEnemySpawner(float deltaTime)
{
    // Spawns more enemies as time passes
    m_EnemySpawnTimer += deltaTime;

    float spawnDelay = GetEnemySpawnDelay();
    while (m_EnemySpawnTimer >= spawnDelay)
    {
        m_EnemySpawnTimer -= spawnDelay;
        CreateEnemy();
        spawnDelay = GetEnemySpawnDelay();
    }
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
    m_EnemySpawnTimer = 0.0f;

    CreatePlayer();
    GetApplication().FollowCamera(player);
    CreateGun();
}

void GameScene::OnUpdate(float deltaTime)
{
    if (!player.IsValid())
    {
        ChangeScene("gameover", 1.0f);
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
    UpdateEnemySpawner(deltaTime);
    UpdateEnemies(deltaTime);
    UpdateHud();
    UpdateBullets(deltaTime);
    UpdatePlayerAnimation(movement);
    UpdatePlayerCollision();
}

void GameScene::OnExit()
{
    bullets.clear();
    enemies.clear();
    Log::Debug("Leaving GameScene");
}
