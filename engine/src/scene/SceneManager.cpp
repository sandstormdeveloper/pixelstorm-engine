#include "pixelstorm/scene/SceneManager.h"

#include "pixelstorm/core/Application.h"
#include "pixelstorm/ecs/World.h"
#include "pixelstorm/systems/PhysicsSystem.h"

SceneManager::SceneManager()
    : m_World(nullptr),
      m_PhysicsSystem(nullptr),
      m_Application(nullptr),
      m_ActiveScene(nullptr),
      m_PendingSceneDelay(0.0f),
      m_HasPendingSceneChange(false)
{
}

SceneManager::SceneManager(World &world)
    : m_World(&world),
      m_PhysicsSystem(nullptr),
      m_Application(nullptr),
      m_ActiveScene(nullptr),
      m_PendingSceneDelay(0.0f),
      m_HasPendingSceneChange(false)
{
}

void SceneManager::SetWorld(World &world)
{
    // Rebinds all registered scenes to the new world
    m_World = &world;

    for (std::pair<const std::string, std::unique_ptr<Scene>> &sceneEntry : m_Scenes)
    {
        BindScene(*sceneEntry.second);
    }
}

void SceneManager::SetPhysicsSystem(PhysicsSystem &physicsSystem)
{
    // Rebinds all registered scenes to the physics system
    m_PhysicsSystem = &physicsSystem;

    for (std::pair<const std::string, std::unique_ptr<Scene>> &sceneEntry : m_Scenes)
    {
        BindScene(*sceneEntry.second);
    }
}

void SceneManager::SetApplication(Application &application)
{
    // Rebinds all registered scenes to the current application
    m_Application = &application;

    for (std::pair<const std::string, std::unique_ptr<Scene>> &sceneEntry : m_Scenes)
    {
        BindScene(*sceneEntry.second);
    }
}

bool SceneManager::AddScene(const std::string &name, std::unique_ptr<Scene> scene)
{
    // Returns if scene name is not valid
    if (name.empty() || !scene)
    {
        return false;
    }

    // Adds scene
    const std::pair<std::unordered_map<std::string, std::unique_ptr<Scene>>::iterator, bool> result =
        m_Scenes.emplace(name, std::move(scene));

    if (result.second && m_World)
    {
        BindScene(*result.first->second);
    }

    return result.second;
}

bool SceneManager::HasScene(const std::string &name) const
{
    // Checks if scene exists
    return m_Scenes.find(name) != m_Scenes.end();
}

bool SceneManager::ChangeScene(const std::string &name)
{
    m_HasPendingSceneChange = false;
    m_PendingSceneName.clear();
    m_PendingSceneDelay = 0.0f;

    return ChangeSceneNow(name);
}

bool SceneManager::ChangeScene(const std::string &name, float delaySeconds)
{
    if (delaySeconds <= 0.0f)
    {
        return ChangeScene(name);
    }

    if (m_HasPendingSceneChange && m_PendingSceneName == name)
    {
        return true;
    }

    if (!HasScene(name) || !m_World)
    {
        return false;
    }

    // Stores the requested scene until the delay expires
    m_PendingSceneName = name;
    m_PendingSceneDelay = delaySeconds;
    m_HasPendingSceneChange = true;
    return true;
}

bool SceneManager::ChangeSceneNow(const std::string &name)
{
    // Looks for the scene by name
    std::unordered_map<std::string, std::unique_ptr<Scene>>::iterator iterator = m_Scenes.find(name);

    // Returns false if the scene or world is not ready
    if (iterator == m_Scenes.end() || !m_World)
    {
        return false;
    }

    // Returns true if the requested scene is already active
    Scene *nextScene = iterator->second.get();
    if (nextScene == m_ActiveScene)
    {
        return true;
    }

    // Calls OnExit for the current scene before switching
    if (m_ActiveScene)
    {
        m_ActiveScene->OnExit();
    }

    // Clears camera follow so the next scene starts with an independent view by default
    if (m_Application)
    {
        m_Application->ResetCameraTracking();
    }

    // Clears the world so the next scene starts clean
    m_World->Clear();

    // Activates the new scene and binds the current context
    m_ActiveSceneName = name;
    m_ActiveScene = nextScene;
    BindScene(*m_ActiveScene);

    // Calls OnEnter for the new scene after the world is ready
    m_ActiveScene->OnEnter();
    return true;
}

void SceneManager::Update(float deltaTime)
{
    // Skips updates if there is no active scene or no world
    if (!m_ActiveScene || !m_World)
    {
        return;
    }

    // Updates the active scene gameplay logic
    m_ActiveScene->OnUpdate(deltaTime);

    if (!m_HasPendingSceneChange)
    {
        return;
    }

    m_PendingSceneDelay -= deltaTime;
    if (m_PendingSceneDelay > 0.0f)
    {
        return;
    }

    const std::string nextSceneName = m_PendingSceneName;
    m_HasPendingSceneChange = false;
    m_PendingSceneName.clear();
    m_PendingSceneDelay = 0.0f;
    ChangeSceneNow(nextSceneName);
}

void SceneManager::Render()
{
    // Skips rendering if there is no active scene or no world
    if (!m_ActiveScene || !m_World)
    {
        return;
    }

    // Lets the active scene draw its own debug or custom content
    m_ActiveScene->OnRender();
}

Scene *SceneManager::GetActiveScene()
{
    // Returns active scene
    return m_ActiveScene;
}

const Scene *SceneManager::GetActiveScene() const
{
    // Returns active scene
    return m_ActiveScene;
}

const std::string &SceneManager::GetActiveSceneName() const
{
    // Returns active scene name
    return m_ActiveSceneName;
}

void SceneManager::BindScene(Scene &scene)
{
    // Connects the scene to the current engine context
    scene.SetContext(*m_World, *this, m_PhysicsSystem, m_Application);
}
