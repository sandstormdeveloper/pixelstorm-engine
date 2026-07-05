#include "pixelstorm/scene/Scene.h"

#include "pixelstorm/core/Application.h"
#include "pixelstorm/scene/SceneManager.h"
#include "pixelstorm/systems/PhysicsSystem.h"

#include <stdexcept>

World &Scene::GetWorld()
{
    if (!m_World)
    {
        throw std::runtime_error("Scene is not connected to a World.");
    }

    return *m_World;
}

const World &Scene::GetWorld() const
{
    if (!m_World)
    {
        throw std::runtime_error("Scene is not connected to a World.");
    }

    return *m_World;
}

SceneManager &Scene::GetScenes()
{
    if (!m_Scenes)
    {
        throw std::runtime_error("Scene is not connected to a SceneManager.");
    }

    return *m_Scenes;
}

const SceneManager &Scene::GetScenes() const
{
    if (!m_Scenes)
    {
        throw std::runtime_error("Scene is not connected to a SceneManager.");
    }

    return *m_Scenes;
}

PhysicsSystem &Scene::GetPhysicsSystem()
{
    // Scenes can only access physics after SceneManager binds the context
    if (!m_PhysicsSystem)
    {
        throw std::runtime_error("Scene is not connected to a PhysicsSystem.");
    }

    return *m_PhysicsSystem;
}

const PhysicsSystem &Scene::GetPhysicsSystem() const
{
    // Scenes can only access physics after SceneManager binds the context
    if (!m_PhysicsSystem)
    {
        throw std::runtime_error("Scene is not connected to a PhysicsSystem.");
    }

    return *m_PhysicsSystem;
}

Application &Scene::GetApplication()
{
    if (!m_Application)
    {
        throw std::runtime_error("Scene is not connected to an Application.");
    }

    return *m_Application;
}

const Application &Scene::GetApplication() const
{
    if (!m_Application)
    {
        throw std::runtime_error("Scene is not connected to an Application.");
    }

    return *m_Application;
}

bool Scene::ChangeScene(const std::string &sceneName)
{
    return GetScenes().ChangeScene(sceneName);
}

void Scene::SetContext(World &world, SceneManager &scenes, PhysicsSystem *physicsSystem, Application *application)
{
    // Stores the engine services that the scene can use during callbacks
    m_World = &world;
    m_Scenes = &scenes;
    m_PhysicsSystem = physicsSystem;
    m_Application = application;
}
