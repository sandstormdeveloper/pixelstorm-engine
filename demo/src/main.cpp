#include "pixelstorm/PixelStorm.h"

int main()
{
    // Creates window
    Application app(640, 360, "Demo");

    // Loads demo sprites from runtime assets
    app.LoadTexture("player", "assets/player.png");
    app.LoadTexture("wall", "assets/wall.png");

    // Gets world handle
    World world = app.GetWorld();

    // Creates player entity with common gameplay components
    Entity player = world.CreateActor(
        "Player",
        Vec2(160.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White()
    );
    player.Sprite().SetTexture("player");

    // Creates wall entity with common static box components
    Entity wall = world.CreateStaticBox(
        "Wall",
        Vec2(240.0f, 180.0f),
        Vec2(32.0f, 32.0f),
        Colors::White()
    );
    wall.Sprite().SetTexture("wall");

    // Update loop
    app.OnUpdate([&](float deltaTime) {
        // Player speed
        const float speed = 120.0f;

        // Gets movement from the built-in combined 2D axis
        const Vec2 movement = Input::GetAxis2D("move");

        // Moves player in pixel coordinates
        player.Transform().Translate(movement * speed * deltaTime);

        // Uses a built-in gameplay action to trigger a one-frame color change
        if (Input::IsActionJustPressed("jump"))
        {
            player.Sprite().SetColor(Colors::Green());
        }

        if (Input::IsActionJustReleased("jump"))
        {
            player.Sprite().SetColor(Colors::White());
        }

        // Moves player to the cursor when left mouse button is pressed
        if (Input::IsMouseButtonJustPressed(MouseButton::Left))
        {
            player.Transform().SetPosition(Input::GetMousePosition());
        }

        // Tints player blue while right mouse button is held
        if (Input::IsMouseButtonDown(MouseButton::Right))
        {
            player.Sprite().SetColor(Colors::Blue());
        }

        // Restores default color when right mouse button is released
        if (Input::IsMouseButtonJustReleased(MouseButton::Right))
        {
            player.Sprite().SetColor(Colors::White());
        }
    });

    // Runs application
    app.Run();

    return 0;
}
