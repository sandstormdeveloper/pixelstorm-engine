#pragma once

#include "pixelstorm/components/Animation.h"

#include <glm/vec2.hpp>

#include <string>

class AnimationProxy
{
public:
    explicit AnimationProxy(Animator &animator)
        : m_Animator(animator)
    {
    }

    void AddClip(const std::string &name, const AnimationClip &clip)
    {
        // Registers or replaces an animation clip by name
        m_Animator.Clips[name] = clip;
    }

    bool HasClip(const std::string &name) const
    {
        // Checks whether the requested animation clip exists
        return m_Animator.Clips.find(name) != m_Animator.Clips.end();
    }

    void RemoveClip(const std::string &name)
    {
        // Removes a clip from the animator
        m_Animator.Clips.erase(name);

        // Clears the active clip if it was removed
        if (m_Animator.CurrentClip == name)
        {
            m_Animator.CurrentClip.clear();
            m_Animator.CurrentFrame = 0;
            m_Animator.Accumulator = 0.0f;
        }
    }

    void Play(const std::string &name)
    {
        // Starts the requested clip from the beginning
        if (!HasClip(name))
        {
            return;
        }

        if (m_Animator.CurrentClip != name)
        {
            m_Animator.CurrentClip = name;
            m_Animator.CurrentFrame = 0;
            m_Animator.Accumulator = 0.0f;
        }

        m_Animator.Playing = true;
    }

    void Play()
    {
        // Resumes the current clip without changing the frame
        if (!m_Animator.CurrentClip.empty())
        {
            m_Animator.Playing = true;
        }
    }

    void Pause()
    {
        // Freezes the current frame
        m_Animator.Playing = false;
    }

    void Stop()
    {
        // Stops playback and rewinds the current clip to its first frame
        m_Animator.Playing = false;
        m_Animator.CurrentFrame = 0;
        m_Animator.Accumulator = 0.0f;
    }

    void Restart()
    {
        // Restarts the active clip from the beginning
        m_Animator.CurrentFrame = 0;
        m_Animator.Accumulator = 0.0f;
        m_Animator.Playing = true;
    }

    bool IsPlaying() const
    {
        // Returns whether the animator currently advances over time
        return m_Animator.Playing;
    }

    const std::string &GetCurrentClip() const
    {
        // Returns the active clip name
        return m_Animator.CurrentClip;
    }

    int GetFrame() const
    {
        // Returns the current frame index inside the active clip
        return m_Animator.CurrentFrame;
    }

    void SetFrame(int frame)
    {
        // Moves the active clip to an explicit frame
        if (frame < 0)
        {
            frame = 0;
        }

        m_Animator.CurrentFrame = frame;
        m_Animator.Accumulator = 0.0f;
    }

    float GetFPS() const
    {
        // Returns the playback speed of the active clip
        const AnimationClip *clip = GetActiveClip();
        return clip ? clip->FPS : 0.0f;
    }

    void SetFPS(float fps)
    {
        // Updates the playback speed of the active clip
        AnimationClip *clip = GetActiveClip();
        if (clip)
        {
            clip->FPS = fps;
        }
    }

    bool IsLooping() const
    {
        // Returns whether the active clip loops
        const AnimationClip *clip = GetActiveClip();
        return clip ? clip->Loop : false;
    }

    void SetLoop(bool loop)
    {
        // Updates whether the active clip loops
        AnimationClip *clip = GetActiveClip();
        if (clip)
        {
            clip->Loop = loop;
        }
    }

    glm::ivec2 GetFrameSize() const
    {
        // Returns the frame size of the active clip
        const AnimationClip *clip = GetActiveClip();
        return clip ? clip->FrameSize : glm::ivec2(0, 0);
    }

    void SetFrameSize(const glm::ivec2 &size)
    {
        // Updates the frame size of the active clip
        AnimationClip *clip = GetActiveClip();
        if (clip)
        {
            clip->FrameSize = size;
        }
    }

    int GetFrameCount() const
    {
        // Returns the number of frames in the active clip
        const AnimationClip *clip = GetActiveClip();
        return clip ? clip->FrameCount : 0;
    }

    void SetFrameCount(int count)
    {
        // Updates the number of frames in the active clip
        AnimationClip *clip = GetActiveClip();
        if (clip)
        {
            clip->FrameCount = count;
        }
    }

    int GetFramesPerRow() const
    {
        // Returns how many frames are stored per row in the spritesheet
        const AnimationClip *clip = GetActiveClip();
        return clip ? clip->FramesPerRow : 0;
    }

    void SetFramesPerRow(int framesPerRow)
    {
        // Updates the frame packing of the active clip
        AnimationClip *clip = GetActiveClip();
        if (clip)
        {
            clip->FramesPerRow = framesPerRow;
        }
    }

private:
    AnimationClip *GetActiveClip()
    {
        // Returns the currently active clip if it exists
        if (m_Animator.CurrentClip.empty())
        {
            return nullptr;
        }

        std::unordered_map<std::string, AnimationClip>::iterator iterator = m_Animator.Clips.find(m_Animator.CurrentClip);
        if (iterator == m_Animator.Clips.end())
        {
            return nullptr;
        }

        return &iterator->second;
    }

    const AnimationClip *GetActiveClip() const
    {
        // Returns the currently active clip if it exists in read-only mode
        if (m_Animator.CurrentClip.empty())
        {
            return nullptr;
        }

        std::unordered_map<std::string, AnimationClip>::const_iterator iterator = m_Animator.Clips.find(m_Animator.CurrentClip);
        if (iterator == m_Animator.Clips.end())
        {
            return nullptr;
        }

        return &iterator->second;
    }

    Animator &m_Animator; // Animator component used by the proxy
};
