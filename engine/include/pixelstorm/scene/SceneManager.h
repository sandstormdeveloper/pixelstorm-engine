#pragma once

#include "pixelstorm/scene/Scene.h"

#include <memory>
#include <string>
#include <unordered_map>

class World;

class SceneManager
{
public:
    SceneManager();
    explicit SceneManager(World &world);

    void SetWorld(World &world);                                          // Sets the world used by scenes
    bool AddScene(const std::string &name, std::unique_ptr<Scene> scene); // Registers a scene by name
    bool HasScene(const std::string &name) const;                         // Returns if scene exists
    bool ChangeScene(const std::string &name);                            // Changes the active scene
    void Update(float deltaTime);                                         // Updates active scene
    void Render();                                                        // Renders active scene hook
    Scene *GetActiveScene();                                              // Returns active scene
    const Scene *GetActiveScene() const;                                  // Returns active scene in read-only mode
    const std::string &GetActiveSceneName() const;                        // Returns active scene name

private:
    void BindScene(Scene &scene); // Connects a scene to the engine context

    World *m_World;                                                   // World used by scenes
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_Scenes; // Registered scenes
    std::string m_ActiveSceneName;                                    // Current scene name
    Scene *m_ActiveScene;                                             // Current scene instance
};
