#include "pixelstorm/PixelStorm.h"
#include "../scenes/GameOverScene.h"
#include "../scenes/MenuScene.h"
#include "../scenes/GameScene.h"

int main()
{
    Application app(640, 360, "PixelStorm");

    app.LoadTexture("player", "assets/player.png");
    app.LoadTexture("gun", "assets/gun.png");
    app.LoadTexture("bullet", "assets/bullet.png");
    UI::Bind(app);

    app.GetScenes().AddScene<MenuScene>("menu");
    app.GetScenes().AddScene<GameScene>("game");
    app.GetScenes().AddScene<GameOverScene>("gameover");
    app.GetScenes().ChangeScene("menu");

    app.Run();

    UI::Unbind();

    return 0;
}
