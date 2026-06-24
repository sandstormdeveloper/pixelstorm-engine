#include "pixelstorm/components/Animation.h"

AnimationClip::AnimationClip()
    : FrameSize(0, 0),
      FrameCount(0),
      FramesPerRow(0),
      FPS(12.0f),
      Loop(true),
      StartFrame(0)
{
}

AnimationClip::AnimationClip(const glm::ivec2 &frameSize, int frameCount, int framesPerRow, float fps, bool loop, int startFrame)
    : FrameSize(frameSize),
      FrameCount(frameCount),
      FramesPerRow(framesPerRow),
      FPS(fps),
      Loop(loop),
      StartFrame(startFrame)
{
}

Animator::Animator()
    : Clips(),
      CurrentClip(),
      Playing(true),
      CurrentFrame(0),
      Accumulator(0.0f)
{
}
