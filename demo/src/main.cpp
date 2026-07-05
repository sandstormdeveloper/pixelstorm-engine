#include "pixelstorm/PixelStorm.h"
#include "../scenes/GameScene.h"
#include "../scenes/SecondScene.h"

int main()
{
    Application app(640, 360, "Demo");

    app.LoadTexture("player_run", "assets/player_run.png");
    app.LoadTexture("wall", "assets/wall.png");
    UI::Bind(app);

    app.GetScenes().AddScene("game", std::make_unique<GameScene>());
    app.GetScenes().AddScene("second", std::make_unique<SecondScene>());
    app.GetScenes().ChangeScene("game");

    app.Run();

    UI::Unbind();

    return 0;
}
