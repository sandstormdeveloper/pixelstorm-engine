#pragma once

#include "pixelstorm/scene/Scene.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <utility>

class World;
class PhysicsSystem;
class Application;

class SceneManager
{
public:
    SceneManager();
    explicit SceneManager(World &world);

    void SetWorld(World &world);                                          // Sets the world used by scenes
    void SetPhysicsSystem(PhysicsSystem &physicsSystem);                  // Sets the physics system used by scenes
    void SetApplication(Application &application);                        // Sets the application used by scenes
    bool AddScene(const std::string &name, std::unique_ptr<Scene> scene); // Registers a scene by name
    template <typename TScene, typename... TArgs,
              typename = std::enable_if_t<std::is_base_of_v<Scene, TScene> && std::is_constructible_v<TScene, TArgs...>>>
    bool AddScene(const std::string &name, TArgs &&...args)
    {
        return AddScene(name, std::make_unique<TScene>(std::forward<TArgs>(args)...));
    }
    bool HasScene(const std::string &name) const;  // Returns if scene exists
    bool ChangeScene(const std::string &name);     // Changes the active scene
    void Update(float deltaTime);                  // Updates active scene
    void Render();                                 // Renders active scene hook
    Scene *GetActiveScene();                       // Returns active scene
    const Scene *GetActiveScene() const;           // Returns active scene in read-only mode
    const std::string &GetActiveSceneName() const; // Returns active scene name

private:
    void BindScene(Scene &scene); // Connects a scene to the engine context

    World *m_World;                                                   // World used by scenes
    PhysicsSystem *m_PhysicsSystem;                                   // Physics system used by scenes
    Application *m_Application;                                       // Application used by scenes
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_Scenes; // Registered scenes
    std::string m_ActiveSceneName;                                    // Current scene name
    Scene *m_ActiveScene;                                             // Current scene instance
};
