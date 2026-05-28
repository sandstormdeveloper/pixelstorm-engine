#pragma once

class SceneManager;
class World;

struct SceneContext
{
    World *GameWorld;     // World used by the active scene
    SceneManager *Scenes; // Scene manager used to request scene changes
};

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void OnEnter(SceneContext &context) {}                   // Called when the scene becomes active
    virtual void OnUpdate(SceneContext &context, float deltaTime) {} // Updates scene gameplay logic
    virtual void OnRender(SceneContext &context) {}                  // Called before the engine presents the frame
    virtual void OnExit(SceneContext &context) {}                    // Called before the scene stops being active
};
