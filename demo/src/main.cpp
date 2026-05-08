#include "pixelstorm/PixelStorm.h"

int main()
{
    // Creates window
    Application app(640, 360, "Demo");

    // Gets world handle
    World world = app.GetWorld();

    // Creates player entity with common gameplay components
    Entity player = world.CreateActor(
        "Player",
        Vec2(160.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::Red()
    );

    // Creates wall entity with common static box components
    Entity wall = world.CreateStaticBox(
        "Wall",
        Vec2(240.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::Blue()
    );

    // Update loop
    app.OnUpdate([&](float deltaTime) {
        // Player speed
        const float speed = 120.0f;

        // Gets movement from named input axis
        const Vec2 movement = Input::GetAxis2D(Axis2D::Move);

        // Moves player in pixel coordinates
        player.Transform().Translate(movement * speed * deltaTime);
    });

    // Runs application
    app.Run();

    return 0;
}
