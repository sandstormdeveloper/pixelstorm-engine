#pragma once

class Time
{
public:
    static void Init();     // Initializes clock
    static void Update();   // Updates clock and delta time
    static void Shutdown(); // Resets clock

    static double GetDeltaTime();   // Returns delta time
    static double GetElapsedTime(); // Returns elapsed time

private:
    static bool s_Initialized;     // True if clock has started
    static double s_DeltaTime;     // Time passed between current and previous frame
    static double s_ElapsedTime;   // Time elapsed
    static double s_LastFrameTime; // Previous frame's time
};
