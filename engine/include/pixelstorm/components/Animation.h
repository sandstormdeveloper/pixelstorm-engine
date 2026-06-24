#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <string>
#include <unordered_map>

struct AnimationClip
{
    glm::ivec2 FrameSize; // Size of a single frame in pixels
    int FrameCount;       // Total number of frames in the clip
    int FramesPerRow;     // Number of frames per row in the spritesheet
    float FPS;            // Playback speed in frames per second
    bool Loop;            // Whether the clip loops when it reaches the end
    int StartFrame;       // First frame index inside the spritesheet

    AnimationClip();
    AnimationClip(const glm::ivec2 &frameSize, int frameCount, int framesPerRow = 0, float fps = 12.0f, bool loop = true, int startFrame = 0);
};

struct Animator
{
    std::unordered_map<std::string, AnimationClip> Clips; // Available animation clips by name
    std::string CurrentClip;                              // Active clip name
    bool Playing;                                         // Whether the active clip advances over time
    int CurrentFrame;                                     // Current frame inside the active clip
    float Accumulator;                                    // Time accumulated toward the next frame advance

    Animator();
};

using AnimationClips = std::unordered_map<std::string, AnimationClip>;
