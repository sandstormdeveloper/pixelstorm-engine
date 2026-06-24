#include "pixelstorm/systems/AnimationSystem.h"

#include "pixelstorm/components/Animation.h"
#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/ecs/Registry.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace
{
    glm::ivec4 BuildFrameSourceRect(const AnimationClip &clip, int frameIndex)
    {
        // Converts a frame index into a source rectangle inside the spritesheet
        if (clip.FrameSize.x <= 0 || clip.FrameSize.y <= 0)
        {
            return glm::ivec4(0, 0, 0, 0);
        }

        const int framesPerRow = std::max(clip.FramesPerRow, 1);
        const int absoluteFrame = clip.StartFrame + frameIndex;
        const int frameX = (absoluteFrame % framesPerRow) * clip.FrameSize.x;
        const int frameY = (absoluteFrame / framesPerRow) * clip.FrameSize.y;
        return glm::ivec4(frameX, frameY, clip.FrameSize.x, clip.FrameSize.y);
    }
}

void AnimationSystem::Update(Registry &registry, float deltaTime)
{
    // Collects entities that own both an animator and a sprite renderer
    std::vector<Entity> animatedEntities = registry.GetEntitiesWith<Animator, SpriteRenderer>();

    // Advances every animated sprite independently
    for (Entity entity : animatedEntities)
    {
        Animator &animator = registry.GetComponent<Animator>(entity);
        SpriteRenderer &sprite = registry.GetComponent<SpriteRenderer>(entity);

        if (animator.CurrentClip.empty())
        {
            continue;
        }

        std::unordered_map<std::string, AnimationClip>::iterator clipIterator = animator.Clips.find(animator.CurrentClip);
        if (clipIterator == animator.Clips.end())
        {
            continue;
        }

        AnimationClip &clip = clipIterator->second;
        if (clip.FrameCount <= 0)
        {
            continue;
        }

        if (animator.Playing && clip.FPS > 0.0f)
        {
            animator.Accumulator += deltaTime;

            const float frameDuration = 1.0f / clip.FPS;
            while (animator.Accumulator >= frameDuration)
            {
                animator.Accumulator -= frameDuration;
                ++animator.CurrentFrame;

                if (animator.CurrentFrame >= clip.FrameCount)
                {
                    if (clip.Loop)
                    {
                        animator.CurrentFrame = 0;
                    }
                    else
                    {
                        animator.CurrentFrame = clip.FrameCount - 1;
                        animator.Playing = false;
                        break;
                    }
                }
            }
        }

        sprite.SourceRect = BuildFrameSourceRect(clip, animator.CurrentFrame);
    }
}
