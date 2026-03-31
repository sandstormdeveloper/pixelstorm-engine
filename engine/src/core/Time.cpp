#include "pixelstorm/core/Time.h"
#include <GLFW/glfw3.h>

// Initializes time variables
bool Time::s_Initialized = false;
double Time::s_DeltaTime = 0.0;
double Time::s_ElapsedTime = 0.0;
double Time::s_LastFrameTime = 0.0;

void Time::Init()
{
    // Gets current time and uses it as last frame time
    s_ElapsedTime = glfwGetTime();
    s_LastFrameTime = s_ElapsedTime;

    // Makes sure delta time starts at 0
    s_DeltaTime = 0.0;

    // Marks clock as started
    s_Initialized = true;
}

void Time::Update()
{
    // Checks clock has started
    if (!s_Initialized)
    {
        Init();
        return;
    }

    // Gets current time
    s_ElapsedTime = glfwGetTime();

    // Calculates delta time
    s_DeltaTime = s_ElapsedTime - s_LastFrameTime;

    // Updates last frame time
    s_LastFrameTime = s_ElapsedTime;
}

void Time::Shutdown()
{
    // Resets time variables
    s_Initialized = false;
    s_DeltaTime = 0.0;
    s_ElapsedTime = 0.0;
    s_LastFrameTime = 0.0;
}

double Time::GetDeltaTime()
{
    // Returns delta time
    return s_DeltaTime;
}

double Time::GetElapsedTime()
{
    // Returns elapsed time
    return s_ElapsedTime;
}
