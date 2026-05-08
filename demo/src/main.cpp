#include "pixelstorm/PixelStorm.h"

int main()
{
    // Creates window
    Application app(640, 360, "Demo");

    // Gets world (entity registry)
    World world = app.GetWorld();

    // Creates player entity and adds components
    Entity player = world.CreateEntity("Player");
    player.AddComponent<Transform>(
        glm::vec2(160.0f, 180.0f),
        glm::vec2(32.0f, 32.0f),
        0.0f
    );
    player.AddComponent<SpriteRenderer>(
        glm::vec4(1.0f, 0.3f, 0.3f, 1.0f)
    );
    player.AddComponent<Collider>(
        glm::vec2(32.0f, 32.0f)
    );
    player.AddComponent<Rigidbody>(
        glm::vec2(0.0f, 0.0f),
        1.0f,
        false
    );

    // Creates wall entity and adds components
    Entity wall = world.CreateEntity("Wall");
    wall.AddComponent<Transform>(
        glm::vec2(240.0f, 180.0f),
        glm::vec2(32.0f, 32.0f),
        0.0f
    );
    wall.AddComponent<SpriteRenderer>(
        glm::vec4(0.3f, 0.6f, 1.0f, 1.0f)
    );
    wall.AddComponent<Collider>(
        glm::vec2(32.0f, 32.0f)
    );
    wall.AddComponent<Rigidbody>(
        glm::vec2(0.0f, 0.0f),
        1.0f,
        true
    );

    // Update loop
    app.SetUpdate([&](float deltaTime) {

        // Gets player transform
        Transform &transform = player.GetComponent<Transform>();

        // Player speed
        const float speed = 120.0f;

        // Right
        if (Input::IsKeyDown(Key::D))
        {
            transform.Position.x += speed * deltaTime;
        }

        // Left
        if (Input::IsKeyDown(Key::A))
        {
            transform.Position.x -= speed * deltaTime;
        }

        // Up
        if (Input::IsKeyDown(Key::W))
        {
            transform.Position.y -= speed * deltaTime;
        }

        // Down
        if (Input::IsKeyDown(Key::S))
        {
            transform.Position.y += speed * deltaTime;
        }
    });

    // Runs application
    app.Run();

    return 0;
}
