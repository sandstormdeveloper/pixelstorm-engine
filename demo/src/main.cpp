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

    // Adds a custom named action on top of the built-in defaults
    Input::AddActionBinding("paint", Key::Space);

    // Update loop
    app.OnUpdate([&](float deltaTime) {
        // Player speed
        const float speed = 120.0f;

        // Gets movement from the built-in combined 2D axis
        const Vec2 movement = Input::GetAxis2D("move");

        // Moves player in pixel coordinates
        player.Transform().Translate(movement * speed * deltaTime);

        // Uses a named action to trigger a one-frame color change
        if (Input::IsActionJustPressed("paint"))
        {
            player.Sprite().SetColor(Colors::Green());
        }

        if (Input::IsActionJustReleased("paint"))
        {
            player.Sprite().SetColor(Colors::Red());
        }
    });

    // Runs application
    app.Run();

    return 0;
}
