#include "pixelstorm/core/Application.h"

int main()
{
    // Creates window (width, height, title)
    Application app(640, 360, "Demo");

    // [Optional] Changes default shader
    // app.SetDefaultShader("default");

    // Gets entity registry
    Registry &registry = app.GetRegistry();

    // Creates player entity
    Entity player = registry.CreateEntity();

    // Adds transform component to player
    registry.AddComponent<Transform>(
        player,
        glm::vec2(-0.5f, 0.0f),
        glm::vec2(0.5f, 0.5f),
        0.0f);

    // Adds sprite renderer component to player
    registry.AddComponent<SpriteRenderer>(
        player,
        app.GetDefaultTexture(),
        glm::vec4(1.0f, 0.3f, 0.3f, 1.0f));

    // Adds collider component to player
    registry.AddComponent<Collider>(
        player,
        glm::vec2(0.5f, 0.5f));

    // Adds rigidbody component to player
    registry.AddComponent<Rigidbody>(
        player,
        glm::vec2(0.0f, 0.0f),
        1.0f,
        false);

    // Creates wall entity
    Entity wall = registry.CreateEntity();

    // Adds transform component to wall
    registry.AddComponent<Transform>(
        wall,
        glm::vec2(0.5f, 0.0f),
        glm::vec2(0.5f, 0.5f),
        0.0f);

    // Adds sprite renderer component to wall
    registry.AddComponent<SpriteRenderer>(
        wall,
        app.GetDefaultTexture(),
        glm::vec4(0.3f, 0.6f, 1.0f, 1.0f));

    // Adds collider component to wall
    registry.AddComponent<Collider>(
        wall,
        glm::vec2(0.5f, 0.5f));

    // Adds rigidbody component to wall
    registry.AddComponent<Rigidbody>(
        wall,
        glm::vec2(0.0f, 0.0f),
        1.0f,
        true);

    // Starts the app
    app.Run();

    return 0;
}