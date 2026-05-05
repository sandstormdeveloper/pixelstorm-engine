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
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.5f, 0.5f),
        0.0f);

    // Adds sprite renderer component to player
    registry.AddComponent<SpriteRenderer>(
        player,
        app.GetDefaultTexture(),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Starts the app
    app.Run();

    return 0;
}