#include "pixelstorm/core/Application.h"

int main()
{
    // Creates window (width, height, title)
    Application app(800, 600, "Demo");

    // [Optional] Changes default shader
    // app.SetDefaultShader("default");

    // Starts the app
    app.Run();

    return 0;
}