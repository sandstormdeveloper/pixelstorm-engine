#include "pixelstorm/core/Application.h"

int main()
{
    Application app(800, 600, "Demo");

    //app.SetDefaultShader("default");

    app.Run();

    return 0;
}