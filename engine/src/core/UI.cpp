#include "pixelstorm/core/UI.h"

#include "pixelstorm/core/Application.h"

Application *UI::s_App = nullptr;

void UI::Bind(Application &app)
{
    // Binds the helper to the active application
    s_App = &app;
}

void UI::Unbind()
{
    // Clears the active application binding
    s_App = nullptr;
}

void UI::Print(const std::string &text, const Vec2 &position, const Color &color, float scale)
{
    // Forwards text drawing to the application if one is bound
    if (s_App)
    {
        s_App->DrawText(text, position, color, scale);
    }
}
