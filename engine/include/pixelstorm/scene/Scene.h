#pragma once

#include <string>

class SceneManager;
class PhysicsSystem;
class World;
class Application;

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void OnEnter() {}                 // Called when the scene becomes active
    virtual void OnUpdate(float deltaTime) {} // Updates scene gameplay logic
    virtual void OnRender() {}                // Called before the engine presents the frame
    virtual void OnExit() {}                  // Called before the scene stops being active

protected:
    World &GetWorld();                              // Returns the world used by this scene
    const World &GetWorld() const;                  // Returns the world in read-only mode
    SceneManager &GetScenes();                      // Returns the scene manager
    const SceneManager &GetScenes() const;          // Returns the scene manager in read-only mode
    PhysicsSystem &GetPhysicsSystem();              // Returns the physics system
    const PhysicsSystem &GetPhysicsSystem() const;  // Returns the physics system in read-only mode
    Application &GetApplication();                  // Returns the application used by this scene
    const Application &GetApplication() const;      // Returns the application in read-only mode
    bool ChangeScene(const std::string &sceneName); // Requests a scene change

private:
    void SetContext(World &world, SceneManager &scenes, PhysicsSystem *physicsSystem, Application *application); // Binds the scene to the engine services it can use

    World *m_World = nullptr;                 // World used by this scene
    SceneManager *m_Scenes = nullptr;         // Scene manager used by this scene
    PhysicsSystem *m_PhysicsSystem = nullptr; // Physics system used by this scene
    Application *m_Application = nullptr;     // Application used by this scene

    friend class SceneManager;
};
