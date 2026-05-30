#include "pixelstorm/scene/SceneManager.h"

#include "pixelstorm/ecs/World.h"

SceneManager::SceneManager()
    : m_World(nullptr),
      m_ActiveScene(nullptr)
{
}

SceneManager::SceneManager(World &world)
    : m_World(&world),
      m_ActiveScene(nullptr)
{
}

void SceneManager::SetWorld(World &world)
{
    // Assigns world for scenes
    m_World = &world;
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

    return result.second;
}

bool SceneManager::HasScene(const std::string &name) const
{
    // Checks if scene exists
    return m_Scenes.find(name) != m_Scenes.end();
}

bool SceneManager::ChangeScene(const std::string &name)
{
    // Looks for scene
    std::unordered_map<std::string, std::unique_ptr<Scene>>::iterator iterator = m_Scenes.find(name);

    // Returns false if scene or world doesn't exist
    if (iterator == m_Scenes.end() || !m_World)
    {
        return false;
    }

    // Returns true if the scene is already loaded
    Scene *nextScene = iterator->second.get();
    if (nextScene == m_ActiveScene)
    {
        return true;
    }

    // Creates context
    SceneContext context = CreateContext();

    // Calls OnExit for active scene
    if (m_ActiveScene)
    {
        m_ActiveScene->OnExit(context);
    }

    // Removes entities from the previous scene before the new scene creates its content
    m_World->Clear();

    // Activates scene
    m_ActiveSceneName = name;
    m_ActiveScene = nextScene;

    // Call OnEnter for new scene
    m_ActiveScene->OnEnter(context);
    return true;
}

void SceneManager::Update(float deltaTime)
{
    // Checks if scene and world exist
    if (!m_ActiveScene || !m_World)
    {
        return;
    }

    // Calls Update for current scene
    SceneContext context = CreateContext();
    m_ActiveScene->OnUpdate(context, deltaTime);
}

void SceneManager::Render()
{
    // Checks if scene and world exist
    if (!m_ActiveScene || !m_World)
    {
        return;
    }

    // Calls Render for current scene
    SceneContext context = CreateContext();
    m_ActiveScene->OnRender(context);
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

SceneContext SceneManager::CreateContext()
{
    // Passes world and scene manager for context
    return {m_World, this};
}
