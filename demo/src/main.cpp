#include "pixelstorm/PixelStorm.h"
#include "../scenes/GameScene.h"
#include "../scenes/SecondScene.h"

int main()
{
    Application app(640, 360, "Demo");

    app.LoadTexture("player", "assets/player.png");
    app.LoadTexture("wall", "assets/wall.png");

    app.GetScenes().AddScene("game", std::make_unique<GameScene>());
    app.GetScenes().AddScene("second", std::make_unique<SecondScene>());
    app.GetScenes().ChangeScene("game");

    app.Run();

    return 0;
}
